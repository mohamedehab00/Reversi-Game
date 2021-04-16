#include <bits/stdc++.h>
using namespace std;

const int N = 8;
const int M = 8;
const int n_players = 2;
char marks[n_players] = {'B', 'W'};
int count_cells[n_players];
char grid[N][M];
int possible_cells[N*M][2];
int possible_cells_size;

//This function prints the grid of Dots-and-Boxes as the game progresses
void print_grid() {
    cout << "Player 1: " << marks[0] << "  vs  Player 2: " << marks[1] << "\n";
    cout << "--";
    for (int i = 0; i < M; cout << "---", i++);
    cout << "--\n";
    for (int i = 0; i < N; i++) {
        cout << "|  ";
        for (int j = 0; j < M; j++)
            cout << grid[i][j] << "  ";
        cout << "|\n";
        cout << "--";
        for (int i = 0; i < M; cout << "---", i++);
        cout << "--\n";
    }
	for (int i = 0; i< n_players; i++)
        cout << "Player " << marks[i] << " is " << count_cells[i] << "\n";
    cout << "--";
    for (int i = 0; i < M; cout << "---", i++);
    cout << "--\n";
}
//This function checks if the grid is full or not
bool check_full() {
	if(count_cells[0]+count_cells[1] == M*N){
		return true;
	}
	else{
		return false;
	}
}
//This function checks if the game has a tie state or not
bool check_player_ability() {
	if(possible_cells_size > 0){
		return true;
	}
	else{
		return false;
	}
}
//This function checks if given cell is chosen or not 
bool check_chosen(int i, int j) {
	for(int row=0;row<possible_cells_size;row++){
		if(possible_cells[row][0] == i && possible_cells[row][1] == j){
			return true;
		}
	}
	return false;
}
//This function checks if given cell is empty or not 
bool check_empty(int i, int j) {
	if(grid[i][j]=='.'){
		return true;
	}
	else{
		return false;
	}
}
//This function checks if given position is valid or not 
bool check_valid_position(int i, int j) {
	if(( 0<=i && i<N ) && ( 0<=j && j<M )){
		return true;
	}
	else{
		return false;
	}
}
//This function calculates the total number of cells for each player
void calc_cells() {
	int B = 0 , W = 0;
	for(int row = 0 ; row < N ; row++){
		for(int col = 0 ; col < M ; col++){
			if(grid[row][col] == 'B'){
				B+=1;
			}
			else if(grid[row][col] == 'W'){
				W+=1;
			}
		}
	}
	count_cells[0] = B;
	count_cells[1] = W; 
}
//This function sets the given mark to the given cell
void set_cell(int i, int j, char mark) {
	grid[i][j] = mark;
}
//This function gets the most cell in the given direction
void get_most_cell(int i, int j, int dx, int dy, bool player, char target_mark, int &most_i, int &most_j) {
    int cnt = 0;
    while (check_valid_position(i, j) && grid[i][j] == marks[1-player]) {
        i += dx;
        j += dy;
        cnt ++;
	}
    if (check_valid_position(i, j) && grid[i][j] == target_mark && cnt > 0)
		most_i = i, most_j = j;
	else
		most_i = -1, most_j = -1;	
}
//This function gets the most cells in the all directions
void get_most_cells(int i, int j, bool player, char target_mark, int most_cells[N*M][2]) {
	int idx = 0;
    int dirs[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};
	for (int k = 0; k < 8; k++) {
		int dx = dirs[k][0], dy = dirs[k][1];
        int most_i, most_j;
		get_most_cell(i+dx, j+dy, dx, dy, player, target_mark, most_i, most_j);
        if (most_i == -1 && most_j == -1)
            continue;
        most_cells[idx][0] = most_i, most_cells[idx][1] = most_j;
		idx ++;
	}
    most_cells[idx][0] = -1, most_cells[idx][1] = -1;
}
//This function switches the neighbor row of the given cell
void switch_row(int i,int j,int y,bool player){
	if(j>y){
		for(int col = y+1 ; col<j+1 ; col++){
			grid[i][col] = marks[player];	
		}
	}
	else if(y>j){
		for(int col = j ; col<y ; col++){
			grid[i][col] = marks[player];	
		}
	}
}
//This function switches the neighbor column of the given cell
void switch_col(int i,int j,int x,bool player){
	if(i>x){
		for(int row = x+1 ; row<i+1 ; row++){
			grid[row][j] = marks[player];	
		}
	}
	else if(x>i){
		for(int row = i ; row<x ; row++){
			grid[row][j] = marks[player];	
		}
	}
}
//This function switches the neighbor diagonal of the given cell
void switch_diag(int i,int j,int x,int y,bool player){
	int col;
	if(i > x && j > y){
		col = y+1;
		for(int row = x+1 ; row<i+1 ; row++){
			grid[row][col] = marks[player];
            col += 1;
		}
	}
	else if(i < x && j < y){
		col = y-1;
		for(int row = x-1 ; row>i-1 ; row--){
			grid[row][col] = marks[player];
            col -= 1;
		}
	}
	else if(i > x && j < y){
		col = y-1;
		for(int row = x+1 ; row<i+1 ; row++){
			grid[row][col] = marks[player];
            col -= 1;
		}
	}
	else if(i < x && j > y){
		col = y+1;
		for(int row = x-1 ; row>i-1 ; row--){
			grid[row][col] = marks[player];
            col += 1;
		}
	}
}
//This function switches the neighbor cells of the given cell
void switch_cells(int i, int j, bool player) {
	int most_cells[N*M][2];
	get_most_cells( i , j , player , marks[player] , most_cells );
	for(int row = 0 ; row < N*M ; row++){
		if(most_cells[row][0] == -1){
			break;
		}
		else{
			if(most_cells[row][0] == i){
				switch_row(i,j,most_cells[row][1],player);
			}
			else if(most_cells[row][1] == j){
				switch_col(i,j,most_cells[row][0],player);
			}
			else{
				switch_diag(i,j,most_cells[row][0],most_cells[row][1],player);
			}
		}
	}
}
//This function generates the possible cells to be chosen 
void generate_possible_cells(bool player) {
	
	possible_cells_size = 0;
	possible_cells[N*M][2];
	
	for( int row = 0 ; row < N ; row++ ){
		for( int col = 0 ; col < M ; col++ ){
			
			int most_cells[N*M][2]={0};
			
			get_most_cells( row , col , player , marks[player] , most_cells );
			
			if(most_cells[0][0] == -1 && most_cells[0][1] == -1){
				
				continue;		
			
			}
			else if(check_empty(row,col)){
				possible_cells[possible_cells_size][0] = row ;
				possible_cells[possible_cells_size][1] = col ;
				possible_cells_size += 1;
			
			}
			
		}	
	}
}
//This function marks the possible cells in the grid
void mark_possible_cells() {
	for(int row=0;row<possible_cells_size;row++){
		int i = possible_cells[row][0];
		int j = possible_cells[row][1];
		grid[i][j] = '?';
	}
}
//This function unmarks the possible cells in the grid
void unmark_possible_cells() {
	for(int row=0;row<possible_cells_size;row++){
		int i = possible_cells[row][0];
		int j = possible_cells[row][1];
		grid[i][j] = '.';
	}
}
//This function clears the game structures
void grid_clear() {
	int x1 = (N-1)/2;int x2 = ((N-1)/2)+1;
	int y1 = (M-1)/2;int y2 = ((M-1)/2)+1;
	
	for(int row = 0 ; row < N ; row++){
		for(int col = 0 ; col < M ; col++){
			if(row == x1){
				if(col == y1){
					grid[row][col] = 'W';
					continue;
				}
				else if(col == y2){
					grid[row][col] = 'B';
					continue;
				}
			}
			else if(row == x2){
				if(col == y1){
					grid[row][col] = 'B';
					continue;
				}
				else if(col == y2){
					grid[row][col] = 'W';
					continue;
				}	
			}
			grid[row][col] = '.';
		}
	}
	
	for(int col = 0 ; col < n_players ; col++){
		count_cells[col] = 0;
	}
	
}
//This function reads a valid position input
void read_input(int &i, int &j) {
    cout << "Enter the row index and column index: ";
    cin >> i >> j;
    while (!check_valid_position(i, j) || !check_empty(i, j) || !check_chosen(i, j)) {
		cout << "Enter a valid two points of the side: ";
		cin >> i >> j;
	}
}
//MAIN FUNCTION
void play_game() {
    cout << "Reversi Game!\n";
    cout << "Welcome...\n";
    cout << "============================\n";
    bool player = 0;
    while (true) {
        //Generate the possible cells to be chosen 
        generate_possible_cells(player);
        //Mark the possible cells
        mark_possible_cells();
        //Prints the grid
        print_grid();
        //Unmark the possible cells
        unmark_possible_cells();
        cout << "Player " << marks[player] << " is playing now\n";
        //Keep the player if there is not an ability with the other player
        if (!check_player_ability()) {
            cout << "Sorry, you can not play on this turn!\n";
            //Player number changes after each turn
            player = 1 - player; 
            continue;
		}
        //Read an input position from the player
        int i, j;
		read_input(i, j);
        //Set the player mark in the input position
        set_cell(i, j, marks[player]);
        //Switch the neighbor cells of the given cell
        switch_cells(i, j, player);
        //Calculates the total number of cells for each player
        calc_cells();
        //Check if the grid has a full state
        if (check_full()) {
            //Prints the grid
            print_grid();
            //Announcement of the final statement
			int max_count_boxes = *max_element(count_cells, count_cells+n_players);
            if (count(count_cells, count_cells+n_players, max_count_boxes) == 1) {
				int idx_max_player = find(count_cells, count_cells+n_players, max_count_boxes) - count_cells;
                cout << "Congrats, Player " << marks[idx_max_player] << " is won!\n";
			}
            else
                cout << "Woah! That's a tie!\n";
            break;
		}
        //Player number changes after each turn
        player = 1 - player;
    }
}
int main() {
	while (true) {
		grid_clear();
		play_game();
    	char c;
    	cout << "Play Again [Y/N] ";
    	cin >> c;
    	if (c != 'y' && c != 'Y')
    		break;
	}
}
