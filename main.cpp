#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <optional>
#include <array>



char cpu_char = 'O';
char your_char = 'X';
int moves_made = 0;

void print_board(char board[9]) {
  for (int i = 0; i < 9; i++) {
    std::cout << board[i];
    if ((i + 1) % 3 == 0)
        std::cout << std::endl;
  }
}


int get_random_choice(char board[9]) {
  std::srand(std::time(nullptr)); 
  std::vector<int> b;   // store indices
  for (int i = 0; i < 9; i++) {
      if (board[i] != 'X' && board[i] != 'O') {
          b.push_back(i);   // push index
      }
  }
  int index = std::rand() % b.size();
  return b[index];   // return the chosen index
}

int get_cpu_choice(char board[9]) {
  /* We want to first win, then block, then go middle if applicable, 
  then connect into two, then go corner, then random */
  // Wins
  // Put diagonals FIRST because they are the best wins (usually)
  std::array<std::array<int, 3>, 8> wins = {{
      {0,4,8}, {2,4,6},            // diagonals
      {0,1,2}, {3,4,5}, {6,7,8},   // rows
      {0,3,6}, {1,4,7}, {2,5,8}    // columns
      
  }};

  // corners
  std::array<int, 4> corners = {0, 2, 6, 8};
  // Get all possible moves
  std::vector<int> b;
  for (int i = 0; i < 9; i++) {
    if (board[i] != 'X' && board[i] != 'O') {
      b.push_back(i);   // push index
    }
  }

  // Check for wins (order of the 2d arr matters here)
  for (auto line : wins) {
    std::vector<int> os;
    for (int i = 0; i < 3; i++) {
      if (board[line[i]] == cpu_char) {
        os.push_back(board[line[i]]);
      }
      
    }
    // Check for a 3rd O for the win
    if (os.size() == 2) {
      for (int i = 0; i < 3; i++) {
        if (board[line[i]] == cpu_char) {
          continue;
        } else if (board[line[i]] != your_char) {
          return line[i];
        }
      }
    }

  }

  // Check for blocks
  for (auto line : wins) {
    std::vector<int> xs;
    for (int i = 0; i < 3; i++) {
      if (board[line[i]] == your_char)
      {
        xs.push_back(board[line[i]]);
      }
      
    }
   // Check for a 3rd X for a block
    if (xs.size() == 2) {
      for (int i = 0; i < 3; i++) {
        if (board[line[i]] == your_char) {
          continue;
        } else if (board[line[i]] != cpu_char) {
          return line[i];
        }
      }
    }

  }

  // Go middle if applicable
  if (board[5] == '5') {
    return 5;
  }

  // try to connect one into two
  for (auto line : wins) {
    std::vector<int> os;
    for (int i = 0; i < 3; i++) {
      if (board[line[i]] == cpu_char) {
        os.push_back(board[line[i]]);
      }
      
    }
    // Check for a 3rd O for the win
    if (os.size() == 1) {
      for (int i = 0; i < 3; i++) {
        if (board[line[i]] == cpu_char) {
          continue;
        } else if (board[line[i]] != your_char) {
          return line[i];
        }
      }
    }
  }

  // Check corners
  for (int corner : corners) {
    if (board[corner] != your_char && board[corner] != cpu_char) {
      return corner;
    }
  }

  return get_random_choice(board);

}


std::optional<bool> check_winner(char board[9]) {
    std::array<std::array<int, 3>, 8> wins = {{
        {0,1,2}, {3,4,5}, {6,7,8},   // rows
        {0,3,6}, {1,4,7}, {2,5,8},   // columns
        {0,4,8}, {2,4,6}             // diagonals
    }};

    for (auto line : wins) {
        if (board[line[0]] == board[line[1]] &&
            board[line[1]] == board[line[2]]) {
            if (board[line[0]] == your_char) {
                return true;   // Player wins
            } else if (board[line[0]] == cpu_char) {
                return false;  // CPU wins
            }
        }
    }

    return std::nullopt; // no winner yet
}


int main() {
  char board[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};

  print_board(board);

  char sq;
  bool found;

  bool first = true;
  std::string turn = "";
  while (turn != "y" && turn != "Y" && turn != "n" && turn != "N") {
    std::cout << "Do you want to go first? (y/n) ";
    std::cin >> turn; 
  }

  if (turn == "n") {
    std::cout << "You are O's\n";
    cpu_char = 'X';
    your_char = 'O';
    int ind = get_cpu_choice(board); // cpu
    board[ind] = cpu_char;
    print_board(board);
  } else {
    std::cout << "You are X's\n";
  }

  while (true) {

    while (!found) {
      std::cout << "Pick a square: ";
      std::cin >> sq;

      for (int i = 0; i < sizeof(board)/sizeof(board[0]); i++) {
          if (board[i] == sq) {
              found = true;
              board[i] = your_char;
              moves_made++;
              break;
          }
      }
    }

    std::optional<bool> wi = check_winner(board);
    if (wi.has_value()) {
      print_board(board);
      std::string winner;
      if (*wi) {
          winner = "Player";
      } else {
          winner = "CPU";
      }

      std::cout << "Winner: " << winner << std::endl;
      return 0;
    }

    if (moves_made == 9) {
       std::cout << "It's a tie!";
       return 0;
    }

    // int ind = get_random_choice(board); // gets random choice
    int ind = get_cpu_choice(board); // cpu
    board[ind] = cpu_char;
    moves_made++;

    print_board(board);

    std::optional<bool> w = check_winner(board);
    if (w.has_value()) {
      std::string winner;
      if (*w) {
        winner = "Player";
      } else {
        winner = "CPU";
      }
      std::cout << "Winner: " << winner << std::endl;
      return 0;
    }

    found = false;

    if (moves_made == 9) {
       std::cout << "It's a tie!";
       return 0;
    }

  }


  return 0;
}
