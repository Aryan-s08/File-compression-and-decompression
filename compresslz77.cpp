#include<bits/stdc++.h>
#include<fstream>
using namespace std;

#define ll long long

int maxlen = 7, maxlenb = 3;
int maxdisp = 32, maxdispb = 5;
queue<char> q;

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

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <input-file>\n";
        return 1;
    }
    string filename = argv[1];
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
    char c;
    while((c = fin.get()) && c != EOF) {
        s.push_back(c);
    }
    cout << "File outputted successfully\n";
    cout << s.size() << ' '  << int(*s.rbegin()) << endl;

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

        // cout << res_dist << ' ' << res_size << ' ' << s[i] << endl;
        if(res_size == 0) {
            fout << to_binary(0, maxlenb);
            q.push(s[i]);
            i++;
            final_size += maxlenb;
        }
        else {
            fout << to_binary(res_size, maxlenb) << to_binary(res_dist - 1, maxdispb);
            i += res_size;
            final_size += maxdispb + maxlenb;
        }
    }

    cout << "Initial size: " << s.size() * 8 << endl;
    cout << "Final size: " << final_size + q.size() * 8 << endl;\
}