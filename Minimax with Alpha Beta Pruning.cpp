#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

//Structures
struct word {
	string label;
	int score;
};
struct letter {
	string label;
	double score;
};
 //Functions
void const printPlayerBoard(vector<letter> player);

void handleInput(); //Gathers input to fill in the letter and word vectors "letters" / "group"

int minimax(vector<letter>& board, int depth, bool maximizingPlayer, int alpha, int beta); //Minimax Algorithm
int getBestMove(vector<letter> board, int turn); //Returns the best Move using the Minimax Algorithm

//Global Variables
vector<letter> player1; //Player 1 Board
vector<letter> player2; //Player 2 Board

int getScoreOfPlayer(vector<letter> player); //Returns the player's score based on their board

vector<letter> letters;//Letters Inputted
vector<word> group; //Words  along with their respective scores inputted

int main()
{
	handleInput();

	vector<letter> board = letters; //Initialize the board

	int turn = 0;
	while (board.size() > 1) {
		int bestMove = getBestMove(board, turn); //Gets the best Move

		if (turn % 2 == 0) {
			player1.push_back(board[bestMove]); //Player 1 will inact the move
		}
		else {
			player2.push_back(board[bestMove]); //Player 2 will inact the move
		}

		board.erase(board.begin() + bestMove); //Remove the letter from the board
		turn++;
	}

	if(turn%2==0)//Adds the last remaining letter
		player1.push_back(board[0]);
	else
		player2.push_back(board[0]);

	cout << player1[0].label << " " << getScoreOfPlayer(player1) << "-" << getScoreOfPlayer(player2);
}

int minimax(vector<letter> & board, int depth, bool maximizingPlayer, int alpha, int beta) {
	if (depth == 0 || board.size() == 1) {
		if (maximizingPlayer) {
			player1.push_back(board[0]);
			int score = getScoreOfPlayer(player1) - getScoreOfPlayer(player2);
			player1.pop_back();
			return score;
		}
		else {
			player2.push_back(board[0]);

			int score = getScoreOfPlayer(player1) - getScoreOfPlayer(player2);
			player2.pop_back();
			return score;
		}
	}
	if (maximizingPlayer) {
		int maxScore = std::numeric_limits<int>::min();
		for (int i = 0; i < 2; i++) {

			letter temp = board[i]; //Does Move
			board.erase(board.begin() + i);
			player1.push_back(temp);
			int val = minimax(board, depth - 1, false, alpha, beta);//Recursively calls minimax
			player1.pop_back();
			board.insert(board.begin() + i, temp);//Reverts Move
			maxScore = max(maxScore, val);
			alpha = max(alpha, val);
			if (beta <= alpha)
				break;
		}
		return maxScore;
	}
	else {
		int minScore = std::numeric_limits<int>::max();
		for (int i = 0; i < 2; i++) {

			letter temp = board[i]; //Does Move
			board.erase(board.begin() + i);
			player2.push_back(temp);
			int val = minimax(board, depth - 1, true, alpha, beta); //Recursively calls minimax
			player2.pop_back();
			board.insert(board.begin() + i, temp);//Reverts Move
			minScore = min(minScore, val);
			beta = min(beta, val);
			if (beta <= alpha)
				break;
		}
		return minScore;
	}
}




void const printPlayerBoard(vector<letter> player) {
	for (unsigned int i = 0; i < player.size(); i++) {
		cout << player[i].label << " ";
	}
	cout << endl;
}

int getScoreOfPlayer(vector<letter> player) { //Checks to see if the player's board can build the alloted words
	int playerScore = 0;		//And if so will return how many points the player has
	for (int i = 0; i < group.size(); i++) {
		int complete = 0;
		for (int j = 0; j < player.size(); j++) {
			size_t temp = (group[i].label).find(player[j].label);
			if (temp != string::npos) {
				complete++;
			}
		}
		if (complete == group[i].label.length()) {
			playerScore += group[i].score;
		}
	}
	return playerScore;
}

int getBestMove(vector<letter> board, int turn) {
	int bestMove = 0;
	int bestScore;
	if (turn % 2 == 0) { 
		bestScore = std::numeric_limits<int>::min();
	}
	else {
		bestScore = std::numeric_limits<int>::max();
	}
	int alpha = std::numeric_limits<int>::min();//Initializing alpha beta variables
	int beta = std::numeric_limits<int>::max();

	for (int i = 0; i < 2; i++) {
		letter temp = board[i]; //Start the Move
		board.erase(board.begin() + i);

		if (turn % 2 == 0) { //If the first player is playing find the highest board score
			player1.push_back(temp);
			int score = minimax(board, board.size() - 1, false, alpha, beta);
			player1.pop_back();
			if (score > bestScore) {
				bestMove = i;
				bestScore = score;
			}
		}
		else { //If second player is playing find the lowest board score
			player2.push_back(temp);
			int score = minimax(board, board.size() - 1, true, alpha, beta);
			player2.pop_back();
			if (score < bestScore) {
				bestMove = i;
				bestScore = score;
			}
		}
		board.insert(board.begin() + i, temp);//Revert the move
	}
	return bestMove; //Return either 0 or 1, signifying whether to take the first or second letter
}

void handleInput() {
	int n;
	int q;
	cin >> n >> q; cin.ignore();
	for (int i = 0; i < n; i++) {
		string x;
		cin >> x; cin.ignore();
		letter temp;
		temp.label = x;
		temp.score = 0.0;
		letters.push_back(temp);
	}
	for (int i = 0; i < q; i++) {
		string z;
		int score;
		cin >> z >> score; cin.ignore();
		word temp;
		temp.label = z;
		temp.score = score;
		group.push_back(temp);
	}
}