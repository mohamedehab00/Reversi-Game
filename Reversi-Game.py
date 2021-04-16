N, M = 8, 8
n_players = 2
marks = ['B', 'W']
count_cells, grid, possible_cells = [], [], []

#This function prints the grid of Reversi as the game progresses
def print_grid():
    print("Player 1: %c  vs  Player 2: %c" % (marks[0], marks[1]))
    print('--' + '---' * M + '--')
    for i in range(N):
        print(end='|  ')
        for j in range(M):
            print(grid[i][j], end='  ')
        print(end='|')
        print()
        print('--' + '---' * M + '--')
    for i in range(n_players):
        print('Player %c has %d cells'% (marks[i], count_cells[i]))
    print('--' + '------' * (M-1))

#This function checks if the game has a full state or not
def check_full():
    if count_cells[0] + count_cells[1] == N * M :
        return True
    else :
        return False 

#This function checks if the game has a tie state or not
def check_player_ability():
    if possible_cells:
        return True
    else:
        return False

#This function checks if given cell is chosen or not 
def check_chosen(i, j):
    if (i,j) in possible_cells:
        return True
    else:
        return False

#This function checks if given cell is empty or not 
def check_empty(i, j):
    if grid[i][j] == ".":
        return True
    else:
        return False

#This function checks if given position is valid or not 
def check_valid_position(i, j):
    if 0<=i<N and 0<=j<M:
        return True
    else:
        return False

#This function calculates the total number of cells for each player
def calc_cells():
    B ,W = 0,0
    for row in range(N):
        B += grid[row].count('B')
        W += grid[row].count('W')
    count_cells[0] = B
    count_cells[1] = W

#This function sets the given mark to the given cell
def set_cell(i, j, mark):
    grid[i][j]=mark

#This function gets the most cell in the given direction
def get_most_cell(i, j, dx, dy, player, target_mark):
    cnt = 0
    while check_valid_position(i, j) and grid[i][j] == marks[1-player]:
        i += dx 
        j += dy
        cnt += 1
    return (i, j) if check_valid_position(i, j) and grid[i][j] == target_mark and cnt > 0 else (-1, -1)

#This function gets the most cells in the all directions
def get_most_cells(i, j, player, target_mark):
    most_cells = []
    dirs = [(1, 0), (-1, 0), (0, 1), (0, -1), (1, 1), (-1, -1), (1, -1), (-1, 1)]
    for dx, dy in dirs:
        most_i,  most_j = get_most_cell(i+dx, j+dy, dx, dy, player, target_mark)
        if most_i == -1 and most_j == -1:
            continue
        most_cells += [(most_i, most_j)]
    return most_cells

def switch_row(i,j,y,player):
    if j > y :
        for col in range(y+1,j+1):
            grid[i][col] = marks[player]
    elif y > j:
        for col in range(j,y):
            grid[i][col] = marks[player]

def switch_col(i,j,x,player):
    if i > x :
        for row in range(x+1,i+1):
            grid[row][j] = marks[player]
    elif x > i :
        for row in range(i,x):
            grid[row][j] = marks[player]

def switch_diag(i,j,x,y,player):
    if i > x and j > y:
        col = y+1
        for row in range(x+1,i+1):
            grid[row][col] = marks[player]
            col += 1
    elif i < x and j < y :
        col = y-1
        for row in range(x-1,i-1,-1):
            grid[row][col] = marks[player]
            col -= 1
    elif i > x and j < y:
        col = y-1
        for row in range(x+1,i+1):
            grid[row][col] = marks[player]
            col -= 1
    elif i < x and j > y:
        col = y+1
        for row in range(x-1,i-1,-1):
            grid[row][col] = marks[player]
            col += 1

#This function switches the neighbor cells of the given cell
def switch_cells(i, j, player):
    points = get_most_cells(i,j,player,marks[player])
    for x , y in points:
        if x == i :
            switch_row(i,j,y,player)
        elif y == j:
            switch_col(i,j,x,player)
        else:
            switch_diag(i,j,x,y,player)

#This function generates the possible cells to be chosen 
def generate_possible_cells(player):
    possible_cells.clear()
    for row in range(N):
        for col in range(M):
            points = get_most_cells(row,col,player,marks[player])
            if points and check_empty(row,col) :
                possible_cells.append((row,col))

#This function marks the possible cells in the grid
def mark_possible_cells():
    for i,j in possible_cells:
        grid[i][j] = "?"

#This function unmarks the possible cells in the grid
def unmark_possible_cells():
    for i,j in possible_cells:
        grid[i][j] = "."

#This function clears the game structures
def grid_clear():
    
    X1 , X2 = (N-1)//2,((N-1)//2)+1
    Y1 , Y2 = (M-1)//2,((M-1)//2)+1
    
    grid.clear()

    count_cells.clear()
    count_cells.append(0)
    count_cells.append(0)
    
    for row in range(N):
        l = ['.']*M
        if row == X1:
            l[Y1] = 'W'
            l[Y2] = 'B'
        elif row == X2:
            l[Y1] = 'B'
            l[Y2] = 'W'
        grid.append(l)


#This function reads a valid position input
def read_input():
    i, j = map(int, input('Enter the row index and column index: ').split())
    while not check_valid_position(i, j) or not check_empty(i, j) or not check_chosen(i, j):
        i, j = map(int, input('Enter a valid row index and a valid column index: ').split())
    return i, j


#MAIN FUNCTION
def play_game():
    print("Reversi Game!")
    print("Welcome...")
    print("============================")
    player = 0
    while True:
        #Generate the possible cells to be chosen 
        generate_possible_cells(player)
        #Mark the possible cells
        mark_possible_cells()
        #Prints the grid
        print_grid()
        #Unmark the possible cells
        unmark_possible_cells()
        print('Player %s is playing now' % marks[player])
        #Keep the player if there is not an ability with the other player
        if not check_player_ability():
            print('Sorry, you can not play on this turn!')
            #Player number changes after each turn
            player = 1 - player 
            continue
        #Read an input position from the player
        i, j = read_input()
        #Set the player mark in the input position
        set_cell(i, j, marks[player])
        # Switch the neighbor cells of the given cell
        switch_cells(i, j, player)
        #Calculates the total number of cells for each player
        calc_cells()
        # #Check if the grid has a full state
        if check_full():
            #Prints the grid
            print_grid()
            #Announcement of the final statement
            if count_cells.count(max(count_cells)) == 1:
                idx_max_player = count_cells.index(max(count_cells))
                print('Congrats, Player %s is won!' % marks[idx_max_player])
            else:
                print("Woah! That's a tie!")
            break
        #Player number changes after each turn
        player = 1 - player 


while True:
	grid_clear()
	play_game()
	c = input('Play Again [Y/N] ')
	if c not in 'yY':
		break
