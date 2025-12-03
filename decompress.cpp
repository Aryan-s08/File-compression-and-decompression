#include <stdio.h>
#include <math.h>
#include<bits/stdc++.h>
#include<fstream>

using namespace std;

int qlen = 0;
char result[10000000];
int rlen = 0;

int bintodec(int *idx, int count, const string &str) {
    int j = 1, n = 0;
    while (j <= count) {
        n = n + ((str[(*idx)++] - '0') * (int) pow(2, count - j));
        j++;
    }
    return n;
}

string extract_filename(const string &filepath) {
    size_t pos = filepath.find_last_of("/\\");
    if (pos == string::npos)
        return filepath;
    return filepath.substr(pos + 1);
}

string filename_without_extension(string filename) {
    string res;
    for(int i = 0; i < filename.size() && filename[i] != '.'; i++)  res.push_back(filename[i]);
    return res;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <input-file>\n";
        return 1;
    }
    string filepath = argv[1];
    string fname = extract_filename(filepath);
    string output_file = fname;
    size_t pos = output_file.find("compressed_");
    if (pos != string::npos) {
        output_file.erase(pos, string("compressed_").length());
    }
    output_file = "decompressed_" + filename_without_extension(output_file) + ".txt";
    ifstream fin(fname);
    if (!fin) {
        cerr << "Error: cannot open input file '" << fname << '\n';
        return 2;
    }
    int size = fin.get();
    int distance = fin.get();
    string q;
    int ch;
    while((ch = fin.get()) != 0) {
        q.push_back(ch);
    }
    string str;
    int c = 0;
    while ((ch = fin.get()) != EOF) {
        for (int i = 0; i < 8; i++) {
            str.push_back(((ch & 128)/128 + '0'));
            ch = ch << 1;

        }
    }
    str.push_back('\0');
    int i = 0;
   while (str[i] != '\0') {
        int size1 = 0, dist1 = 0, l = 0;
        size1 = bintodec(&i, size, str);
        if (size1 == 0) {
            result[rlen++] = q[qlen++];
            continue;
        }
        dist1 = bintodec(&i, distance, str);
        while (l < size1) {
            result[rlen] = result[rlen - dist1 - 1];
            rlen++;
            l++;
        }
    }
    result[rlen] = '\0';
    ofstream fout(output_file, ios::trunc);
    if (!fout) {
        cerr << "Error: cannot open output file '" << output_file << "' for writing.\n";
        return 4;
    }
    fout << result;
    fout.close();
    return 0;
    
}