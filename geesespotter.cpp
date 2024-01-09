#include "geesespotter_lib.h"
#include <iostream>

char *create_board(std::size_t x_dim, std::size_t y_dim){
// Allocate a 1-dimensional char array with x_dim * y_dim elements and initialize each element with zero. Note
//that the initialization value is the value 0 and not the character '0'. The data type of the array is char not
//because ASCII characters are going to be stored in each element, but because char is an 8-bit data type.
//See Data Structure section above for details

// using std::size_t as size_t usually matches the processor address size and we won't run into issues of using 
// 32 bits int for example and it being less than you need
    std::size_t elements = x_dim * y_dim;

    // Allocate a 1-dimensional char array with x_dim * y_dim elements
    // board is a pointer to the first element of the array
    char* board = new char[elements];

    // initialize each element with zero. 
//Note that the initialization value is the value 0 and not the character '0'.
// to do this we use a for loop as we can go into each entry of the array 
    for (std::size_t i = 0; i < elements; ++i) {
        board[i] = 0; }
        // we created the board now lets return it to use it
    return board;

}
// Deallocate the given board - clears that board from memory
void clean_board(char *board){ 
// deallocates the memory associated with the object
delete[] board; 
// clean board so make 0
board = nullptr;

}

// Prints the content of the board to std::cout. Each field is represented with a single character. If a field is
//hidden this function prints an asterisk ‘*’, if a field is marked an ‘M’ is printed, and otherwise the field value
//is displayed ('0' to '9'). Note that the only time a field with a goose is displayed is at the end of the game
//(either won or lost). There are no spaces between printed board elements.
//Refer to the Data Structure section below for further information on how information is encoded in each
//array element
void print_board(char *board, std::size_t x_dim, std::size_t y_dim){

for (std::size_t y = 0; y < y_dim; ++y) {
        for (std::size_t x = 0; x < x_dim; ++x) {

            // so this is flattening a two-dimensional array into a one-dimensional array
            // access an element in 1d array as if it was 2D based on the given row and column
            char element = board[y * x_dim + x];

            // checks if the bitwise & operation between element and the bit mask returned by marked_mask()
            // is 0 and if it is the bits specified by marked_mask are in the element
            // marked tile uses 'M'
        if (element & marked_mask()) {
                std::cout << "M";
            
            } 
            // checks if the bitwise & operation between element and the bit mask returned by hidden_mask()
            //is 0 and if it is then the bits spefifcied are in the element
            // hidden tile so use * as instructed 
            else if (element & hidden_mask()) {
                std::cout << "*";

        // last case is just the tiles with numbers 1-8
                // cast char into a int
            } 
            else {
                //int how_many = element & value_mask();
                std::cout << (int(element & value_mask()));
             
    
            }
        }
        std::cout << std::endl;
}
}

// iterate through all the elements and set each element to value returned by the hidden_mask()
void hide_board(char *board, std::size_t x_dim, std::size_t y_dim) {
for (std::size_t i = 0; i < x_dim * y_dim; ++i) {
        board[i]  |= hidden_mask();
        //BOARD[I] = BOARD[I] | HIDDEN_MASK()
    }
}

// Toggles the marked state of the tile at (x=x_loc, y=y_loc).
//If the tile is already revealed, does nothing and returns 2.
//Otherwise, returns 0
int mark(char *board, std::size_t x_dim, std::size_t y_dim, std::size_t x_loc, 
std::size_t y_loc){

// calc the index of the element in 1D array that corresponds to the 2D coordinates
//allows you to access the specific element of the board at the coordinates
// using char reference variable element read the value in the array at the coordinates
    char& element = board[y_loc * x_dim + x_loc];

    // checks whether the tile is hidden
    // if the result of this operation is not 0 (so hidden) the condition is true
    if (element & hidden_mask()) {
        // uses xor to flip the marked state bit without affecting other bits
        element ^= marked_mask(); 
        // successfully toggled
        return 0;
    }
    // stays the same
    return 2; 

}

void compute_neighbours(char *board, std::size_t x_dim, std::size_t y_dim){
   // so give neighboring cells their relative coordinates 
   // there are eight possible directions
    int horizontal [] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    int vertical [] = { -1, -1, -1, 0, 0, 1, 1, 1 };

   for (std::size_t y = 0; y < y_dim; ++y) {
        for (std::size_t x = 0; x < x_dim; ++x) {
            if (board[y * x_dim + x] != 9) {
                int goose_num = 0;

                for (int i = 0; i < 8; ++i) {
                    int now_x = x + horizontal[i];
                    int now_y = y + vertical[i];

                    if (now_x >= 0 && now_x < x_dim && now_y >= 0 && now_y < y_dim) {
                        if (board[now_y * x_dim + now_x] == 9) {
                            goose_num++;
                        }
                    }
                }

                board[y * x_dim + x] = goose_num;
            }
        }
    }
}

// Helper function to compute neighbors
int compute_neighbours_helper(char *board, std::size_t x_dim, std::size_t y_dim) {
    int unrevealed = 0;

    for (std::size_t y = 0; y < y_dim; ++y) {
        for (std::size_t x = 0; x < x_dim; ++x) {
            char element = board[y * x_dim + x];
            if ((element & value_mask()) != 9 && !(element & hidden_mask())) {
                ++unrevealed;
            }
        }
    }

    return unrevealed;
}

bool is_game_won(char *board, std::size_t x_dim, std::size_t y_dim){
bool revealed = true;

    for (std::size_t y = 0; y < y_dim; ++y) {
        for (std::size_t x = 0; x < x_dim; ++x) {
            char element = board[y * x_dim + x];
            if ((element & value_mask()) != 9 && (element & (hidden_mask() | marked_mask()))) {

                revealed = false;
                break;
            }
        }

        if (!revealed) {
            break;
        }
    }

    return revealed;
}

int reveal(char *board, std::size_t x_dim, std::size_t y_dim, std::size_t x_loc,
std::size_t y_loc){
char &element = board[y_loc * x_dim + x_loc];

    if (!(element & hidden_mask()) && !(element & marked_mask())) {

        // field is already revealed so return 2.
        return 2;
    }

    if (element & marked_mask()) {
        // field is marked so return 1.
        return 1;
    }

    if (element & value_mask() == 9) {
        // goose spotted so reveal it and return 9.
        // clear the hidden bit
        element &= ~hidden_mask(); 
        return 9;
    }

    // field is empty so contains a value of zero
    // reveal it and neighbors
    // clear the hidden bit
    element &= ~hidden_mask(); 

    if (element & value_mask() == 0) {

        // empty field then reveal neighboring tiles
        for (int y = -1; y <= 1; ++y) {
            for (int x = -1; x <= 1; ++x) {
                int new_x = static_cast<int>(x_loc) + x;
                int new_y = static_cast<int>(y_loc) + y;

                if (new_x >= 0 && new_x < static_cast<int>(x_dim) && new_y >= 0 && new_y < static_cast<int>(y_dim)) {
                    if (element & value_mask() == 0) {
                        // only reveal neighbors that aren't marked and successfully marked
                        int reveal_result = reveal(board, x_dim, y_dim, new_x, new_y);
                        
                        if (reveal_result == 9) {
                            // goose is revealed then stop revealing neighbors
                            return 9;
                        }
                    }
                }
            }
        }
    }

    return 0;
}