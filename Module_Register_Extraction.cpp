#include <iostream>
#include <fstream>
#include <vector>
#include <regex>


// This struct contains relevant information regarding a register. (i.e., name, bitwidth etc.)
struct reg_det{
    std::string name;
    std::string arr_value;
    int bitwidth;
};

// A class containing all the module information. This class will be used to mutate the seeds.
class module_info{
private:
    //  Rrgister detail containers
    std::vector<reg_det> module_input, module_output;


    std::vector <reg_det> reg_name_separation(std::string line, std::string delimiter, std::regex pattern){
        std::vector<reg_det> out_list;
        size_t pos = 0;
        reg_det token;
        std::string reg_names, arr_size;

        // Array pattern to calculate the size of a register in a design;
        std::regex arr_pattern{"\\[\\s*(\\d)\\s*:\\s*(\\d)\\s*\\]"};
        std::smatch match, len_match;

        if(std::regex_search(line, match, pattern)){
            arr_size = match[2];
            token.arr_value = arr_size;
            if (arr_size.length() == 0) token.bitwidth = 1;
            else {
                std::regex_search(arr_size, len_match, arr_pattern);
                token.bitwidth = std::abs(std::stoi(len_match[2]) - std::stoi(len_match[1])) + 1;
            }
            reg_names = match[3];
            if(reg_names.find(delimiter) == std::string::npos){
                token.name = reg_names;
                out_list.push_back(token);
            } else {
                while((pos = reg_names.find(delimiter)) != std::string::npos){
                    token.name = reg_names.substr(0, pos);
                    token.name = token.name;
                    reg_names.erase(0, pos+delimiter.length());
                    out_list.push_back(token);
                }
            }
        }
        return out_list;
    }

public:
    void get_inouts(std::string line){
        /*
         * This function will read each line and find input and outputs of the module.
         */
        

        // Pattern for input declaration
        std::regex input_pattern{"\\s*input\\s*(reg\\s*)?(\\[[:\\d\\w]+\\])?([\\w\\d,{}]*)"};
        // Pattern for output declaration
        std::regex output_pattern{"\\s*output\\s*(reg\\s*)?(\\[[:\\d\\w]+\\])?([\\w\\d,{}]*)"};

        std::string delimiter = ",";    // Delimiter string

        std::smatch mtch;
        std::vector<reg_det> temp;

        if(std::regex_search(line, mtch, input_pattern)){
            temp = reg_name_separation(line, delimiter, input_pattern);
            module_input.insert(module_input.end(), temp.begin(), temp.end());
        } else if(std::regex_search(line, mtch, output_pattern)){
            temp = reg_name_separation(line, delimiter, output_pattern);
            module_output.insert(module_output.end(), temp.begin(), temp.end());
        }
    }

    void show_inouts(){
        std::cout << "Input Pins:" << std::endl;
        for (reg_det x : module_input){
            std::cout << "\t" << x.name << "\t" << x.arr_value << "\t" << x.bitwidth << std::endl;
        }
        std::cout << "Output Pins:" << std::endl;
        for (reg_det x : module_output){
            std::cout << "\t" << x.name << "\t" << x.arr_value << "\t" << x.bitwidth << std::endl;
        }
    }
};

int main(int argc, char** argv) {
    std::string line;

    module_info get_io;

    std::ifstream infile(argv[1]);

    while (std::getline(infile, line)){
        get_io.get_inouts(line);
    }

    get_io.show_inouts();

    return 0;
}
