#include <iostream>
#include "GameController.h"
#include <fstream>
#include <string>

bool GameController::play(BlockFall& game, const string& commands_file) {

    // TODO: Implement the gameplay here while reading the commands from the input file given as the 3rd command-line
    //       argument. The return value represents if the gameplay was successful or not: false if game over,
    //       true otherwise.
    fstream command_file;
    command_file.open(commands_file, ios::in);
    if (command_file.is_open()) {
        string line;

        game.active_rotation = game.initial_block;
        int row_size = game.active_rotation->shape.size();
        int column_size = game.active_rotation->shape[0].size();
        int temp_column = column_size;
        int temp_row = row_size;

        int another_column;
        int custom_column = 0;
        int empty = 0;
        int count = 0;
        int column_change = 0;
        string adding_block;

        string gravity_mode = to_string(game.gravity_mode_on);

        while (getline(command_file, line)) {

            int a = 0;
            int full_row = 0;


            for(int i = 0; i < game.grid.size(); i++){
                for(int j = 0; j < game.grid[i].size(); j++){
                    if(game.grid[i][j] == 1){
                        full_row++;
                    }
                }
                if(full_row == game.grid[i].size()){
                    for(int k = i; 0 < k; k--){
                        for(int j = 0; j < game.grid[i].size(); j++){
                            game.grid[k][j] = game.grid[k - 1][j];
                        }
                    }
                    for(int j = 0; j < game.grid[i].size(); j++){
                        game.grid[0][j] = 0;
                    }
                }
                full_row = 0;
            }

            if (adding_block != "Block Added") {
                for (int i = 0; i < column_size; i++) {
                    if (game.grid[row_size - 1][i] == 1) {
                        count++;
                        break;
                    }
                }

                if (count == 1) {
                    break;
                }

                for (int i = 0; i < row_size; i++) {
                    for (int j = 0; j < column_size; j++) {
                        game.grid[i][j] = game.active_rotation->shape[i][j];
                    }
                }

                adding_block = "Block Added";
                column_change++;
            }


            if (line == "ROTATE_LEFT") {
                for (int k = 0; k < temp_row; k++) {
                    for (int j = column_change - 1; j < temp_column; j++) {
                        game.grid[k][j] = 0;
                    }
                }
                game.active_rotation = game.active_rotation->left_rotation;
                row_size = game.active_rotation->shape.size();
                column_size = game.active_rotation->shape[0].size();
                temp_column = column_size;
                temp_row = row_size;

                for (int i = 0; i < row_size; i++) {
                    if (game.grid[i][temp_column] == 1) {
                        game.active_rotation = game.active_rotation->right_rotation;
                        row_size = game.active_rotation->shape.size();
                        column_size = game.active_rotation->shape[0].size();
                        temp_column = column_size;
                        temp_row = row_size;
                        continue;
                    }
                }

                if (temp_column == game.grid[0].size()) {
                    game.active_rotation = game.active_rotation->right_rotation;
                    row_size = game.active_rotation->shape.size();
                    column_size = game.active_rotation->shape[0].size();
                    temp_column = column_size;
                    temp_row = row_size;
                    continue;
                }

                for (int i = 0; i < temp_row; i++) {
                    for (int j = column_change - 1; j < temp_column; j++) {
                        game.grid[i][j] = game.active_rotation->shape[i][a];
                        a++;
                    }
                    a = 0;
                }

            } else if (line == "ROTATE_RIGHT") {
                for (int k = 0; k < temp_row; k++) {
                    for (int j = column_change; j < temp_column + 1; j++) {
                        game.grid[k][j - 1] = 0;
                    }
                }

                temp_column = temp_column - column_size;

                game.active_rotation = game.active_rotation->right_rotation;
                row_size = game.active_rotation->shape.size();
                column_size = game.active_rotation->shape[0].size();
                temp_column = temp_column + column_size;
                temp_row = row_size;

                for (int i = 0; i < row_size; i++) {
                    if (game.grid[i][temp_column] == 1) {
                        game.active_rotation = game.active_rotation->left_rotation;
                        row_size = game.active_rotation->shape.size();
                        column_size = game.active_rotation->shape[0].size();
                        temp_row = row_size;
                        continue;
                    }
                }

                if (temp_column == game.grid[0].size() + column_size) {
                    game.active_rotation = game.active_rotation->left_rotation;
                    row_size = game.active_rotation->shape.size();
                    column_size = game.active_rotation->shape[0].size();
                    temp_row = row_size;
                    continue;
                }

                for (int i = 0; i < temp_row; i++) {
                    for (int j = column_change; j < temp_column + 1; j++) {
                        game.grid[i][j - 1] = game.active_rotation->shape[i][a];
                        a++;
                    }
                    a = 0;
                }


            } else if (line == "MOVE_RIGHT") {
                for (int i = 0; i < row_size; i++) {
                    if (game.grid[i][temp_column] == 1) {
                        continue;
                    }
                }

                if (temp_column == game.grid[0].size()) {
                    continue;
                }

                for (int i = 0; i < temp_row; i++) {
                    game.grid[i][column_change - 1] = 0;
                }

                for (int i = 0; i < temp_row; i++) {
                    for (int j = column_change; j < temp_column + 1; j++) {
                        game.grid[i][j] = game.active_rotation->shape[i][a];
                        a++;
                    }
                    a = 0;
                }

                column_change++;
                temp_column++;

            } else if (line == "MOVE_LEFT") {
                for (int i = 0; i < row_size; i++) {
                    if (game.grid[i][temp_column] == 1) {
                        continue;
                    }
                }

                if (temp_column == column_size) {
                    continue;
                }

                for (int i = 0; i < temp_row; i++) {
                    game.grid[i][temp_column - 1] = 0;
                }

                for (int i = 0; i < temp_row; i++) {
                    for (int j = column_change; j < temp_column + 1; j++) {
                        game.grid[i][j - 2] = game.active_rotation->shape[i][a];
                        a++;
                    }
                    a = 0;
                }

                column_change--;
                temp_column--;
            } else if (line == "DROP") {
                if (gravity_mode == "0") {
                    for(int k = 0; k < temp_row; k++){
                        for(int j = column_change - 1; j < temp_column; j++){
                            game.grid[k][j] = 0;
                        }
                    }

                    int putting_row = game.grid.size() - 1;
                    int getting_other_column = 0;


                    for (int i = column_change - 1; i < temp_column; i++) { //111/010 sıçıyor onu düzelt kanzi(10. blokta problem vars)
                        for (int k = 0; k < game.grid.size(); k++) {
                            for(int j = temp_row - 1; 0 <= j; j--){
                                if (game.grid[k][i] == 1 && game.active_rotation->shape[j][a] == 1) {
                                    if (putting_row >= k) {
                                        putting_row = k - 1;
                                    }
                                    getting_other_column++;
                                    break;
                                }
                                else if(game.grid[k][i] == 1 && game.active_rotation->shape[j][a] == 0){
                                    if(putting_row >= k){
                                        putting_row = k;
                                    }
                                    getting_other_column++;
                                    break;
                                }
                                else if(game.grid[k][i] == 0 && game.active_rotation->shape[j][a] == 1){
                                    if(putting_row < k && j != 0){
                                        if(game.active_rotation->shape[j - 1][a] == 1){
                                            getting_other_column++;
                                            break;
                                        }
                                    }

                                    else if(putting_row < k){
                                        putting_row = k;
                                    }
                                }
                            }
                            if(getting_other_column > 0){
                                getting_other_column = 0;
                                break;
                            }
                        }
                        a++;

                    }

                    a = 0;
                    for(int j = temp_row - 1; 0 <= j; j--){
                        for(int k = column_change - 1; k < temp_column; k++){
                            if(game.grid[putting_row][k] != 1){
                                game.grid[putting_row][k] = game.active_rotation->shape[j][a];
                            }
                            a++;
                        }
                        a = 0;
                        putting_row--;
                    }

                } else if (gravity_mode == "1") { //GRAVITY_MODE SIKINTILI
                    for (int k = 0; k < temp_row; k++) {
                        for (int j = column_change - 1; j < temp_column; j++) {
                            game.grid[k][j] = 0;
                        }
                    }


                    for(int i = column_change - 1; i < temp_column; i++){
                        for(int k = 0; k < game.grid.size(); k++){
                            if(game.grid[k][i] == 1){
                                int putting_row = k - 1;
                                for(int j = row_size - 1; 0 <= j; j--){
                                    if(game.active_rotation->shape[j][a] == 1){
                                        game.grid[putting_row][i] = game.active_rotation->shape[j][a];
                                        putting_row--;
                                    }
                                }
                                a++;
                                break;
                            }
                        }
                    }

                    a = 0;

                }
                game.active_rotation = game.active_rotation->next_block;
                row_size = game.active_rotation->shape.size();
                column_size = game.active_rotation->shape[0].size();
                temp_column = column_size;
                temp_row = row_size;
                empty = 0;
                count = 0;
                column_change = 0;

                adding_block = "Initializing New Block";


            }

            else if (line == "GRAVITY_SWITCH") {
                if (gravity_mode == "0") {
                    gravity_mode = "1";
                } else if (gravity_mode == "1") {
                    gravity_mode = "0";
                }

            }

        }

    }
    return false;

}



