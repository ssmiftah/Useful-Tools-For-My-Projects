#include <iostream>
#include <regex>
#include <fstream>
#include <filesystem>

void remove_comments(std::string fileName, std::string outFileName){
  /*
  This function parses a Verilog/SystemVerilog file, removes the comments and empty lines
  it takes the input and output file names. It does not have a return value.
  */

    std::string line;

    std::ifstream inFile(fileName);
    std::ofstream outFile;


    std::regex inline_Comment_pattern{"\\s*//.*"};          // Inline comment pattern
    std::regex block_Comment_strt_pattern{"/\\*.*"};        // Block comment start pattern 
    std::regex block_Comment_end_pattern{".*\\*/"};         // Block comment end pattern
    std::regex blank_line_pattern{"\\s*"};                  // Empty Line pattern
    std::smatch match, mtch;

    bool commentStrt = 0;                                   // Flag to determine if it is a comment or not

    outFile.open(outFileName);
    while (std::getline(inFile, line)){
        if (std::regex_search(line, match, block_Comment_strt_pattern)){
            commentStat = 1;
            line = std::regex_replace(line, block_Comment_strt_pattern, "");
        } else if (std::regex_search(line, match, block_Comment_end_pattern)){
            line = std::regex_replace(line, block_Comment_end_pattern, "");
            commentStat = 0;
        }
        // Writing into the output file without any comments
        if (!commentStrt){      // If it is not a comment write into the output, otherwise ignore.
            if (std::regex_search(line, match, inline_Comment_pattern)){
                line = std::regex_replace(line, inline_Comment_pattern, "");   
            }
        } else line = "";       // If a line is comment replace it with a blank string
        if (!std::regex_match(line, mtch, blank_line_pattern)) outFile << line << std::endl;    // If line number is not empty, write it to the new file
    }
    outFile.close();
    

}

int main(int argc, char** argv){

    remove_comments(argv[1], argv[2]);

    return 0;
}
