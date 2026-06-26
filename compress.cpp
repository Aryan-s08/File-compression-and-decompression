#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <bitset>
#include <string>
#include <algorithm>
#include <cstring>
#include <functional>

using namespace std;
using ll = long long;
using uc = unsigned char;

const int BUFFER_SIZE = 8192;
uc output_queue_buffer[BUFFER_SIZE];
bitset<BUFFER_SIZE> output_buffer;
const int MAX_LIMIT_ON_LENGTH_BITS = 15;
const int MAX_LIMIT_ON_DISP_BITS = 24;
ofstream output_file, output_queue_file;
int output_buffer_index = 0, output_queue_buffer_index = 0;
string output_filename, output_queue_filename, final_output_filename;

ifstream read_file(string filename) {
    ifstream file(filename, ios::binary);
    if(!file) {
        cerr << "Could not open input file\n";
        exit(2);
    }
    return file;
}

void create_files(string filename) {
    for(int i = 0; ; i++) {
        output_filename = filename, output_queue_filename = filename + "_queue";
        final_output_filename = filename;
        if(i > 0) {
            output_filename = output_filename + '(' + to_string(i) + ')' + ".aca";
            output_queue_filename = output_queue_filename + '(' + to_string(i) + ')' + ".aca";
            final_output_filename = final_output_filename + '(' + to_string(i) + ')' + ".compressed";
        }
        else {
            output_filename = output_filename + ".aca";
            output_queue_filename = output_queue_filename + ".aca";
            final_output_filename = final_output_filename + ".compressed";
        }
        if(!filesystem::exists(output_filename) && !filesystem::exists(output_queue_filename) && !filesystem::exists(final_output_filename))  break;
    }

    output_file.open(output_filename, ios::out | ios::binary | ios::trunc);
    if(!output_file) {
        cerr << "Could not open output file\n";
        exit(3);
    }
    output_queue_file.open(output_queue_filename, ios::out | ios::trunc | ios::binary);
    if(!output_queue_file) {
        cerr << "Could not open queue output file\n";
        filesystem::remove(output_filename);
        exit(4);
    }
}

int get_integer(string s, int def, int lim) {
    try {
        int k = stoi(s);
        if(k <= 0 || lim < k) {
            cout << "Value above max limit " << lim << endl;
            cout << "Using default value: " << def << endl;
            return def;        
        }
        return k;
    }
    catch (exception const& ex) {
        cout << ex.what() << endl;
        cout << "Using default value: " << def << endl;
        return def;
    }
}

void clear_output_buffer() {
    for(int i = 0; i < output_buffer_index; i += 8) {
        unsigned char currentByte = 0;
        for(int j = 0; j < 8; j++) {
            if(output_buffer[i + j]) {
                currentByte |= (1 << j);
            }
        }
        output_file.put(currentByte);
    }
    output_buffer_index = 0;
    output_buffer.reset();
}

void clear_output_queue_buffer() {
    output_queue_file.write(reinterpret_cast<char *>(output_queue_buffer), output_queue_buffer_index);
    output_queue_buffer_index = 0;
}

void write_in_buffer(int val, int sz) {
    for(int i = sz - 1; i >= 0; i--) {
        if(output_buffer_index == BUFFER_SIZE) {
            clear_output_buffer();
        }
        output_buffer[output_buffer_index++] = (1 & (val >> i));
    }
}

void write_in_queue(uc c) {
    if(output_queue_buffer_index >= BUFFER_SIZE) {
        clear_output_queue_buffer();
    }
    output_queue_buffer[output_queue_buffer_index++] = c;
}

void combine_files(int max_length_bits, int max_disp_bits) {
    ofstream final_output_file(final_output_filename, ios::out | ios::trunc | ios::binary);
    if(!final_output_file) {
        cerr << "File creation failed\n";
        filesystem::remove(output_filename);
        filesystem::remove(output_queue_filename);
        exit(5);
    }
    
    final_output_file.put(static_cast<unsigned char>(max_length_bits));
    final_output_file.put(static_cast<unsigned char>(max_disp_bits));

    uint32_t queue_size = filesystem::file_size(output_queue_filename);
    final_output_file.put(static_cast<unsigned char>((queue_size >> 0) & 0xFF));
    final_output_file.put(static_cast<unsigned char>((queue_size >> 8) & 0xFF));
    final_output_file.put(static_cast<unsigned char>((queue_size >> 16) & 0xFF));
    final_output_file.put(static_cast<unsigned char>((queue_size >> 24) & 0xFF));

    ifstream queue_in(output_queue_filename, ios::binary);
    if(queue_in) {
        final_output_file << queue_in.rdbuf();
        queue_in.close();
    }

    ifstream bitstream_in(output_filename, ios::binary);
    if(bitstream_in) {
        final_output_file << bitstream_in.rdbuf();
        bitstream_in.close();
    }

    final_output_file.close();
    filesystem::remove(output_filename);
    filesystem::remove(output_queue_filename);
    
    cout << "Compression successful: " << final_output_filename << endl;
}

int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if(argc != 4) {
        cerr << "Usage: " << argv[0] << ' ' << "<input-file> <max-characters-bits> <max-distance-bits>\n";
        return 1;
    }

    ifstream input_file = read_file(argv[1]);
    int max_length_bits = get_integer(argv[2], 4, MAX_LIMIT_ON_LENGTH_BITS);
    int max_disp_bits = get_integer(argv[3], 14, MAX_LIMIT_ON_DISP_BITS);
    int max_length = (1 << max_length_bits) - 1;
    int max_disp = (1 << max_disp_bits);
    create_files(argv[1]);

    const int HASH_BITS = 15;
    const int HASH_SIZE = 1 << HASH_BITS;
    vector<int> head(HASH_SIZE, -1);
    vector<int> prev_match(max_disp, -1);

    int window_size = max_disp;
    int mem_buffer_size = window_size * 2 + max_length;
    vector<uc> buffer(mem_buffer_size);

    long long buffer_offset = -window_size;
    int pos = window_size; 

    input_file.read(reinterpret_cast<char*>(buffer.data() + window_size), mem_buffer_size - window_size);
    int valid_data = window_size + input_file.gcount();
    bool eof = (input_file.gcount() < mem_buffer_size - window_size);

    function<int(int)> get_hash = [&](int p) {
        return ((buffer[p] << 10) ^ (buffer[p+1] << 5) ^ buffer[p+2]) & (HASH_SIZE - 1);
    };

    while (pos < valid_data) {
        int best_len = 0;
        int best_dist = 0;
        int abs_pos = buffer_offset + pos;

        if (valid_data - pos >= 3) {
            int h = get_hash(pos);
            int match_abs_pos = head[h];
            int chain_len = 100;

            while (match_abs_pos != -1 && chain_len-- > 0) {
                int dist = abs_pos - match_abs_pos;
                
                if (dist > max_disp || dist <= 0) break; 

                int match_pos = match_abs_pos - buffer_offset;
                int len = 0;
                int max_possible_len = min({(int)max_length, valid_data - pos, valid_data - match_pos});

                while (len < max_possible_len && buffer[match_pos + len] == buffer[pos + len]) {
                    len++;
                }

                if (len > best_len) {
                    best_len = len;
                    best_dist = dist;
                    if (best_len == max_length) break;
                }
                
                match_abs_pos = prev_match[match_abs_pos & (max_disp - 1)];
            }
        }

        int min_match_required = max(3, (max_disp_bits + 7) / 8);

        if (best_len < min_match_required) {
            write_in_buffer(0, max_length_bits);
            write_in_queue(buffer[pos]);
            
            if (valid_data - pos >= 3) {
                int h = get_hash(pos);
                prev_match[abs_pos & (max_disp - 1)] = head[h];
                head[h] = abs_pos;
            }
            pos++;
        } else {
            write_in_buffer(best_len, max_length_bits);
            write_in_buffer(best_dist - 1, max_disp_bits);
            
            for (int k = 0; k < best_len; k++) {
                if (valid_data - (pos + k) >= 3) {
                    int curr_abs = abs_pos + k;
                    int h = get_hash(pos + k);
                    prev_match[curr_abs & (max_disp - 1)] = head[h];
                    head[h] = curr_abs;
                }
            }
            pos += best_len;
        }

        if (!eof && valid_data - pos <= max_length) {
            int shift = pos - window_size;
            
            memmove(buffer.data(), buffer.data() + shift, valid_data - shift);
            valid_data -= shift;
            buffer_offset += shift;

            pos -= shift;
            
            input_file.read(reinterpret_cast<char*>(buffer.data() + valid_data), mem_buffer_size - valid_data);
            int bytes_read = input_file.gcount();
            valid_data += bytes_read;
            
            if (bytes_read == 0) eof = true;
        }
    }

    clear_output_buffer();
    clear_output_queue_buffer();
    input_file.close();
    output_file.close();
    output_queue_file.close();
    combine_files(max_length_bits, max_disp_bits);
    return 0;
}