#pragma once
#include <fstream>
#include <iostream>
#include <set>
#include <string>

#include "archive/read.cpp"
#include "unarchive/read.cpp"

void Create(std::string path) {
    CreateFile(path);
}

void List(std::string path) {
    std::set<std::string> file_names;
    FileList(path, file_names);
    for (auto name : file_names) {
        std::cout << "<" << name << ">" << '\n';
    }
}

void Extract(std::string archive_path, std::vector<std::string> names) {
    ReadArchive(archive_path, names);
}

void Append(std::string archive_path, std::string file_path) {
    AddFile(file_path, archive_path);
}

void Delete(std::string archive_path, std::string file_path) {
    DeleteFile(archive_path, file_path);
}

void Concatenate(std::string in_archive1, std::string in_archive2, std::string out_archive) {
    std::string out_archive_name = GetFileName(out_archive);
    std::string in_archive1_full = "../" + in_archive1 + ".haf";
    std::string out_archive_full = "../" + out_archive + ".haf";

    std::fstream out;
    out.open(out_archive_full, std::ifstream::out | std::ifstream::binary | std::ifstream::app);
    std::fstream in;

    in.open(in_archive1_full, std::ifstream::in | std::ifstream::binary);

    while (true) {
        unsigned char c;
        if (!in.good()) {
            break;
        }
        c = in.get();
        if (c == 255) {
            break;
        }
        out << c;
    }

    in.close();

    out.close();
    MergeArchives(out_archive, in_archive2);
}
