#include "BlockFall.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(
        gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}



void BlockFall::read_blocks(const string &input_file) {
    // TODO: Read the blocks from the input file and initialize "initial_block" and "active_rotation" member variables
    Block block = *new Block();
    fstream file;
    file.open(input_file, ios::in);
    if(file.is_open()){
        vector<string> lines;
        string line, text;
        while(getline(file, line)){
            if(!line.empty() && line.back() != ']' && line.at(0) == '[' ){
                text += "[" + line + "]" + ",";
            }
            else if(!line.empty() && line.back() != ']' && line.at(0) != '['){
                text += "[" + line + "]" + ",";
            }
            else if(!line.empty() && line.back() == ']' && line.at(0) != '['){
                text += "[" + line + "]";
                lines.push_back(text);
                text = "";
            }
            else if(!line.empty() && line.back() == ']' && line.at(0) == '['){
                text = line;
                lines.push_back(text);
                text = "";
            }
        }


        for(int i = 0; i < lines.size(); i++){
            size_t startPos = lines[i].find_first_of("[");
            while (startPos != string::npos) {
                size_t endPos = lines[i].find_first_of("]", startPos + 1);
                if (endPos != string::npos) {
                    string rowString = lines[i].substr(startPos + 1, endPos - startPos - 1);
                    istringstream rowStream(rowString);
                    vector<bool> row;
                    char ch;
                    while(rowStream.get(ch)){
                        if (isdigit(ch)) {
                            bool value = static_cast<bool>(ch - '0');
                            row.push_back(value);
                        }
                    }

                    block.shape.push_back(row);


                }
                startPos = lines[i].find_first_of("[", endPos + 1);
            }

            if(i == 0){
                initial_block = new Block;
                initial_block->shape = block.shape;
                initial_block->next_block = nullptr;
                active_rotation = initial_block;
                active_rotation->right_rotation = nullptr;
                active_rotation->left_rotation = nullptr;


                vector<vector<bool>> originalBlock = block.shape;
                while(active_rotation->right_rotation != initial_block){
                    while(active_rotation->right_rotation != nullptr){
                        active_rotation = active_rotation->right_rotation;
                    }
                    int m = block.shape.size();    // Number of rows
                    int n = block.shape[0].size(); // Number of columns

                    if(m == 1 && n == 1){
                        active_rotation->right_rotation = active_rotation;
                        active_rotation->right_rotation->left_rotation = active_rotation;
                        break;
                    }

                    else if(m == 1 && n > 1){

                        vector<vector<bool>> rotatedMatrix(n, vector<bool>(m, 0));
                        for(int j = 0; j < n; ++j){
                            rotatedMatrix[j][0] = block.shape[0][j];
                        }

                        if(rotatedMatrix != originalBlock){
                            block.shape = rotatedMatrix;
                            active_rotation->right_rotation = new Block;
                            active_rotation->right_rotation->shape = rotatedMatrix;
                            active_rotation->right_rotation->right_rotation = initial_block;
                            active_rotation->right_rotation->left_rotation = active_rotation;
                            active_rotation->right_rotation->right_rotation->left_rotation = active_rotation->right_rotation;
                            break;
                        }
                    }

                    else{
                        // Create a new matrix with dimensions swapped
                        vector<vector<bool>> rotatedMatrix(n, vector<bool>(m, 0));

                        // Fill the rotated matrix with the values from the original matrix
                        for (int i = 0; i < m; ++i) {
                            for (int j = 0; j < n; ++j) {
                                rotatedMatrix[j][m - 1 - i] = block.shape[i][j];
                            }
                        }

                        if(rotatedMatrix != originalBlock){
                            block.shape = rotatedMatrix;

                            active_rotation->right_rotation = new Block;
                            active_rotation->right_rotation->shape = rotatedMatrix;
                            active_rotation->right_rotation->right_rotation = nullptr;
                            active_rotation->right_rotation->left_rotation = active_rotation;

                        }

                        else{
                            block.shape = rotatedMatrix;
                            active_rotation->right_rotation = initial_block;
                            active_rotation->right_rotation->left_rotation = active_rotation;
                            active_rotation = initial_block;
                            break;
                        }
                    }


                }
            }

            else if(i != lines.size() - 1){
                Block *newBlock = new Block;
                newBlock->shape = block.shape;
                newBlock->next_block = nullptr;

                // If initial_block is nullptr, set it to the newly created block
                if (initial_block == nullptr) {
                    initial_block = newBlock;
                    active_rotation = initial_block;
                } else {
                    // Otherwise, add the new block to the end of the list
                    Block *temp = initial_block;
                    while (temp->next_block != nullptr) {
                        temp = temp->next_block;
                    }
                    temp->next_block = newBlock;
                }

                while(active_rotation->right_rotation->next_block != newBlock){
                    active_rotation->right_rotation->next_block = newBlock;
                    active_rotation = active_rotation->right_rotation;
                }

                active_rotation = newBlock;
                active_rotation->right_rotation = nullptr;
                active_rotation->left_rotation = nullptr;

                vector<vector<bool>> originalBlock = block.shape;
                while(active_rotation->right_rotation != newBlock){
                    Block * temp2 = active_rotation;
                    while(temp2->right_rotation != nullptr){
                        temp2 = temp2->right_rotation;
                    }
                    active_rotation = temp2;


                    int m = block.shape.size();    // Number of rows
                    int n = block.shape[0].size(); // Number of columns

                    // Create a new matrix with dimensions swapped
                    vector<vector<bool>> rotatedMatrix(n, vector<bool>(m, 0));

                    // Fill the rotated matrix with the values from the original matrix
                    for (int i = 0; i < m; ++i) {
                        for (int j = 0; j < n; ++j) {
                            rotatedMatrix[j][m - 1 - i] = block.shape[i][j];
                        }
                    }

                    if(rotatedMatrix != originalBlock){
                        block.shape = rotatedMatrix;
                        active_rotation->right_rotation = new Block;
                        active_rotation->right_rotation->shape = rotatedMatrix;
                        active_rotation->right_rotation->right_rotation = nullptr;
                        active_rotation->right_rotation->left_rotation = active_rotation;
                    }

                    else{
                        block.shape = rotatedMatrix;
                        active_rotation->right_rotation = newBlock;
                        active_rotation->right_rotation->left_rotation = active_rotation;
                        active_rotation = active_rotation->right_rotation;
                        break;

                    }

                }
            }

            else if(i == lines.size() - 1){
                power_up = block.shape;
            }

            block.shape.clear();
        }
    }


    // TODO: For every block, generate its rotations and properly implement the multilevel linked list structure
    //       that represents the game blocks, as explained in the PA instructions.
    // TODO: Initialize the "power_up" member variable as the last block from the input file (do not add it to the linked list!)
}

void BlockFall::initialize_grid(const string &input_file) {
    // TODO: Initialize "rows" and "cols" member variables

    fstream file;
    file.open(input_file, ios::in);
    if (file.is_open()){
        vector<string> lines;
        string line;
        while(getline(file, line)){
            istringstream iss(line);
            vector<int> numbers;
            int num;
            lines.push_back(line);

            while(iss >> num){
                numbers.push_back(num);
            }
            grid.push_back(numbers);
        }

        rows = lines.size();
        cols = int(lines[0].size()) - (int(lines[0].size()) / 2);
    }
    // TODO: Initialize "grid" member variable using the command-line argument 1 in main

}



BlockFall::~BlockFall() {
    // TODO: Free dynamically allocated memory used for storing game blocks

}
