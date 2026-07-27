#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <optional>
#include <array>





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


std::optional<bool> check_winner(char board[9]) {
    std::array<std::array<int, 3>, 8> wins = {{
        {0,1,2}, {3,4,5}, {6,7,8},   // rows
        {0,3,6}, {1,4,7}, {2,5,8},   // columns
        {0,4,8}, {2,4,6}             // diagonals
    }};

    for (auto line : wins) {
        if (board[line[0]] == board[line[1]] &&
            board[line[1]] == board[line[2]]) {
            if (board[line[0]] == 'X') {
                return true;   // Player wins
            } else if (board[line[0]] == 'O') {
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
  while (true) {

    while (!found) {
      std::cout << "Pick a square: ";
      std::cin >> sq;

      for (int i = 0; i < sizeof(board)/sizeof(board[0]); i++) {
          if (board[i] == sq) {
              found = true;
              board[i] = 'X';
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

    int ind = get_random_choice(board);
    board[ind] = 'O';

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

  }

  return 0;
}

