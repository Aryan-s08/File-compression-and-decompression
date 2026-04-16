#include <stdio.h>
#include <string.h>
#include <bits/stdc++.h>
#include <fstream>
using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <input-file>\n";
        return 1;
    }
    string f1name = argv[1];
    string f2name = argv[2];
    ifstream fin(f1name);
    if (!fin) {
        cerr << "Error: cannot open input file '" << f1name << '\n';
        return 2;
    }

    string f1, f2;
    int ch;
    while((ch = fin.get()) != EOF) {
        f1.push_back(char(ch));
    }
    fin.close();
    fin.open(f2name);
    if (!fin) {
        cerr << "Error: cannot open queue file '" << f2name << '\n';
        return 2;
    }
    while((ch = fin.get()) != EOF) {
        f2.push_back(char(ch));
    }
    fin.close();
    if (f1 != f2) {
        printf("Files are not the same\n\n");
    } else {
        printf("Files are the same\n\n");
    }

}
