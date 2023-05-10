#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <map>

std::string highlight(const std::string& line) {
    std::map<std::string, std::regex> patterns = {
        {"comment", std::regex("(#.*)")},
        {"keyword", std::regex("\\b(class|def|if|else|elif|for|while|return|import|from|as|try|except|finally|with|in|is|and|or|not|print|lambda|yield)\\b")},
        {"literal", std::regex("(\\b\\d+\\b|'[^']*'|\"[^\"]*\")")},
        {"operator", std::regex("([+\\-*/%=<>!]=?|\\*\\*|//|\\+=|\\-=)")},
        {"variable", std::regex("\\b([a-zA-Z_][a-zA-Z0-9_]*)\\b")},
    };

    std::string highlighted_line = "<span>";
    for (const auto& [category, pattern] : patterns) {
        size_t offset = 0;
        std::smatch match;
        while (std::regex_search(line.begin() + offset, line.end(), match, pattern)) {
            std::string pre_match = match.prefix().str();
            if (!pre_match.empty()) {
                highlighted_line += pre_match;
                highlighted_line += "</span><span>";
            }
            std::string match_str = match.str();
            std::string replacement = "<span class=\"" + category + "\">" + match_str + "</span>";
            highlighted_line += replacement;
            offset += match.position() + match_str.length();
        }
        highlighted_line += "</span>";
    }
    return highlighted_line;
}

void convertToHtml(const std::string& input_file, const std::string& output_file) {
    std::ifstream in(input_file);
    if (!in) {
        std::cerr << "Error: Cannot open input file." << std::endl;
        return;
    }

    std::ofstream out(output_file);
    if (!out) {
        std::cerr << "Error: Cannot open output file." << std::endl;
        return;
    }

    out << "<!DOCTYPE html>\n<html>\n<head>\n<style>\n"
        << "pre { font-family: monospace; }\n"
        << ".comment { color: #8E908C; }\n"
        << ".keyword { color: #8959A8; font-weight: bold; }\n"
        << ".literal { color: #718C00; }\n"
        << ".operator { color: #3E999F; }\n"
        << ".variable { color: #4271AE; }\n"
        << "</style>\n</head>\n<body>\n<pre>\n";

    std::string line;
    while (std::getline(in, line)) {
        out << highlight(line) << '\n';
    }

    out << "</pre>\n</body>\n</html>\n";
}

int main() {
    const std::string input_file("input.py");
    const std::string output_file("output.html");

    convertToHtml(input_file, output_file);

    return 0;
}
