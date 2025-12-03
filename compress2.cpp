#include<bits/stdc++.h>
#include<fstream>
using namespace std;

#define ll long long

int maxlen, maxdisp, maxdispb, maxlenb;string q;
const int buff_size = (1<<28);
const ll MOD = 1e9 + 7;
const ll base1 = 5689;
const ll base2 = 8861;
map<pair<int, int>, int> hesh;
vector<ll> p1, p2, pow1v, pow2v;
int buff_index = 0, n = -1;
unsigned char buffer = 0;
string s, result;

void write_bits(int value, int total_bits) {
    while(total_bits > 0) {
        if(buff_index == 8) {
            result.push_back(buffer);
            // cout << (int)buffer << " om\n";
            buff_index = 0;
        }
        int y = (value & (1 << (--total_bits))), x = 7 - buff_index;
        buffer = (y) ? (buffer | (1 << x)) : (buffer & (~(1 << x)));
        buff_index++;
    }
}

string file_name_without_extension(string& filename) {
    string res;
    for(int i = 0; i < filename.size() && filename[i] != '.'; i++)  res.push_back(filename[i]);
    return res;
}

string read_file(string filename) {
    ifstream file(filename, ios::binary);
    if (!file) { cerr << "Cannot open file\n"; return ""; }
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    return content;
}

void build_hashes() {
    if(n == -1) {
        cout << "Input string first\n";
        return;
    }
    p1.assign(n+1, 0);
    p2.assign(n+1, 0);
    pow1v.assign(n+1, 1);
    pow2v.assign(n+1, 1);
    for (int i = 0; i < n; ++i) {
        p1[i+1] = ((p1[i] * base1) % MOD + (unsigned char)s[i]) % MOD;
        p2[i+1] = ((p2[i] * base2) % MOD + (unsigned char)s[i]) % MOD;
        pow1v[i+1] = pow1v[i] * base1 % MOD;
        pow2v[i+1] = pow2v[i] * base2 % MOD;
    }
}

pair<int, int> compute_hash(int l, int r) {
    ll x1 = (p1[r+1] - (p1[l] * pow1v[r-l+1]) % MOD + MOD) % MOD;
    ll x2 = (p2[r+1] - (p2[l] * pow2v[r-l+1]) % MOD + MOD) % MOD;
    return {(int)x1, (int)x2};
}

string extract_filename(const string &filepath) {
    size_t pos = filepath.find_last_of("/\\");
    if (pos == string::npos)
        return filepath;
    return filepath.substr(pos + 1);
}

int main(int argc, char* argv[]) {
    auto start = std::chrono::high_resolution_clock::now();
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " <input-file>\n";
        return 1;
    }
    string filepath = argv[1];
    string filename = extract_filename(filepath);
    maxlenb = stoi(argv[2]);
    maxdispb = stoi(argv[3]);
    maxlen = (1 << maxlenb) - 1;
    maxdisp = (1 << maxdispb);

    s = read_file(filename);
    n = s.size();
    ll initial_size = s.size() * 8ll, final_size = 24;
    build_hashes();
    string output_filename = "compressed_" + file_name_without_extension(filename) + ".bin";
    FILE* output_file = fopen(output_filename.c_str(), "wb");
    q.push_back(maxlenb);
    q.push_back(maxdispb);

    for(int i = 0; i < s.size(); i++) {
        int max_copy_len = min(maxlen, n - i);
        int lo = 2, hi = max_copy_len, bestlen = 0, bestpos = -1;

        while(lo <= hi) {
            int mid = (lo + hi) / 2;
            pair<int, int> p = compute_hash(i, i + mid - 1);

            int found = -1;
            for(int j = max(0, i - maxdisp); j <= i - mid; j++) {
                if(compute_hash(j, j + mid - 1) == p) {
                    found = j;
                    break;
                }
            }
            if(~found) {
                bestlen = mid;
                bestpos = found;
                lo = mid + 1;
            }
            else {
                hi = mid - 1;
            }
        }

        if(bestlen > 1) {
            write_bits(bestlen, maxlenb);
            write_bits(i - bestpos - 1, maxdispb);
            final_size += maxlenb + maxdispb;
            // cout << bestlen << ' ' << i - bestpos - 1 << ' ' << buff_index << endl;
            i += bestlen - 1;
        }
        else {
            write_bits(0, maxlenb);
            q.push_back(s[i]);
            final_size += maxlenb + 8;
            // cout << i << ' ' << buff_index << endl;
        }
    }

    q.push_back('\0');
    fwrite(q.c_str(), 1, q.size(), output_file);
    for(int i = 8 - buff_index; i < 16; i++)    {
        write_bits(0, 1);
        // cout << i << ' ' << (int)buffer << endl;
    }
    // write_bits(0, 8 - buff_index);
    result.push_back('\0');
    fwrite(result.c_str(), 1, result.size(), output_file);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    cout << "File compressed successfully!!!\n";
    cout << "Initial size: " << initial_size << endl;
    cout << "Final size: " << final_size << endl;
    cout << "Percentage compressed: " << (long double)(initial_size - final_size) * 100 / initial_size << "%\n";
    cout << "Time taken: " << duration.count() << " ms" << endl;
}
