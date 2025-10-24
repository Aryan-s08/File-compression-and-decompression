#include<bits/stdc++.h>
#include<fstream>
using namespace std;

#define ll long long

int maxlen, maxdisp, maxdispb, maxlenb;
queue<char> q;
vector<string> vd, vs;

string to_binary(int x, int sz) {
    string res;
    while(x) {
        if(x & 1)   res.push_back('1');
        else        res.push_back('0');
        x >>= 1;
    }
    int t = res.size();
    for(int i = 0; i < sz - t; i++) res.push_back('0');
    reverse(res.begin(), res.end());
    return res;
}

void precompute() {
    vd.resize(maxdisp + 10);
    vs.resize(maxlen + 10);

    for(int i = 0; i < maxdisp; i++)   vd[i] = to_binary(i, maxdispb);
    for(int i = 0; i <= maxlen; i++)   vs[i] = to_binary(i, maxlenb);
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " <input-file>\n";
        return 1;
    }
    string filename = argv[1];
    maxlenb = stoi(argv[2]);
    maxdispb = stoi(argv[3]);
    maxlen = (1 << maxlenb) - 1;
    maxdisp = (1 << maxdispb);
    precompute();

    ifstream fin(filename);
    if (!fin) {
        cerr << "Error: cannot open input file '" << filename << "'\n";
        return 2;
    }
    string output_file = "compressed_" + filename;
    ofstream fout(output_file);
    if (!fout) {
        cerr << "Error: cannot create output file '" << output_file << "'\n";
        return 3;
    }

    int final_size = 0;
    string s;
    int ch;
    while((ch = fin.get()) != EOF) {
        s.push_back(char(ch));
    }
    cout << "File outputted successfully\n";
    cout << "Size of input: " << s.size() << endl;

    for(int i = 0; i < s.size(); ) {
        int res_size = 0, res_dist = 0;
        for(int ind = max(0, i - maxdisp); ind < i; ind++) {
            int curr_size = 0;
            for(int j = ind; j < min(i, ind + maxlen); j++) {
                if(i + j - ind < s.size() && s[j] == s[i + (j - ind)]) {
                    curr_size++;
                }
                else {
                    break;
                }
            }
            if(curr_size > res_size) {
                res_size = curr_size;
                res_dist = i - ind;
            }
        }

        if(res_size < (maxdispb + 7) / 8) {
            fout << vs[0];
            q.push(s[i]);
            i++;
            final_size += maxlenb;
        }
        else {
            fout << vs[res_size] << vd[res_dist - 1];
            i += res_size;
            final_size += maxdispb + maxlenb;
        }
    }

    int initial_size = 8 * s.size();
    final_size += q.size() * 8;

    cout << "Initial size: " << initial_size << endl;
    cout << "Final size: " << final_size << endl;
    cout << "Percentage compressed: " << (long double) (initial_size - final_size) / initial_size * 100 << endl;

    fout.close();
    string output_queue = "compressed_queue_" + filename ;
    fout.open(output_queue);
    if (!fout) {
        cerr << "Error: cannot create output file '" << output_queue << "'\n";
        return 4;
    }

    while(q.size()) {
        fout << q.front();
        q.pop();
    }
}
