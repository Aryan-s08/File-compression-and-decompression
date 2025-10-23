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

int main(int argc, char *argv[]) {
    if (argc < 5) {
        cerr << "Usage: " << argv[0] << " <input-file>\n";
        return 1;
    }
    int i = 0;
    string fname = argv[1];
    string qname = argv[2];
    int size = stoi(argv[3]);
    int distance = stoi(argv[4]);
    ifstream fin(fname);
    if (!fin) {
        cerr << "Error: cannot open input file '" << fname << '\n';
        return 2;
    }
    string str;
    int ch;
    while((ch = fin.get()) != EOF) {
        str.push_back(char(ch));
    }
    fin.close();
    ifstream fin1(qname);
    if (!fin1) {
        cerr << "Error: cannot open queue file '" << fname << '\n';
        return 2;
    }
    string q;
    while((ch = fin1.get()) != EOF) {
        q.push_back(char(ch));
    }
    fin1.close();
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
    printf("%s",result);
    ofstream fout(fname, ios::trunc);
    if (!fout) {
        cerr << "Error: cannot open output file '" << fname << "' for writing.\n";
        return 4;
    }
    fout << result;
    fout.close();
    return 0;
}