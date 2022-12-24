//Parker Hendry
//11-20-22
/*
This program implements a backtracking algorithm to solve a Puzzle Peg game.
*/

import java.util.ArrayList;

public class PuzzlePegs {
 
 	//ints to hold beginning and ending position
	static int begin, end;
	
	//ArrayList object to hold board history
	static ArrayList<char[]> history;	
	//ArrayList object to hold move history
	static ArrayList<String> moveHist;
	
	//2D array to hold all possible moves in a Puzzle Peg game
	static int[][] moves = 
		{
			{1, 2, 4},
			{1, 3, 6},
			{2, 4, 7},
			{2, 5, 9},
			{3, 5, 8},
			{3, 6, 10},
			{4, 2, 1},
			{4, 5, 6},
			{4, 7, 11},
			{4, 8, 13},
			{5, 8, 12},
			{5, 9, 14},
			{6, 3, 1},
			{6, 5, 4},
			{6, 9, 13},
			{6, 10, 15},
			{7, 4, 2},
			{7, 8, 9},
			{8, 5, 3},
			{8, 9, 10},
			{9, 5, 2},
			{9, 8, 7},
			{10, 6, 3},
			{10, 9, 8},
			{11, 7, 4},
			{11, 12, 13},
			{12, 8, 5},
			{12, 13, 14},
			{13, 12, 11},
			{13, 8, 4},
			{13, 9, 6},
			{13, 14, 15},
			{14, 13, 12},
			{14, 9, 5},
			{15, 10, 6},
			{15, 14, 13}
		};
 
	public static void main(String[] args)
	{
		//Implement board as 1D array
		char[] board = {' ','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o'};
		
		//Create ArrayList object to hold board history
		history = new ArrayList<>();
		//Create ArrayList object to hold move history
		moveHist = new ArrayList<>();
		
		if (args.length == 0)
		{
			//If no command line arguments, default to start position 13 with any ending position
		
			board[13] = '.';
			end = 16;	
		}
		else if (args.length == 1)
		{
			//If 1 command line argument, parse to int and use that as start position with any ending position
		
			begin = Integer.parseInt(args[0]);
			
			if (begin < 1 || begin > 15)
			{
				System.out.println("The starting position must be between 1 and 15.");
				return;
			}
		
			board[begin] = '.';
			end = 16;
		}
		else if (args.length == 2)
		{
			//If 2 command line arguments, parse both to int and use the first as the start position 
			//and the second as the ending position
		
			begin = Integer.parseInt(args[0]);
			
			if (begin < 1 || begin > 15)
			{
				System.out.println("The starting position must be between 1 and 15.");
				return;
			}
		
			board[begin] = '.';
			
			end = Integer.parseInt(args[1]);
			
			if (end < 1 || end > 15)
			{
				System.out.println("The ending position must be between 1 and 15.");
				return;
			}	
		}
		else 
		{
			System.out.println("Please enter 1, 2, or no command line arguments.");
			return;
		}
		
		//Print initial board
		System.out.println("```");
		System.out.println("Initial board");		
		print(board);
		
		//Attempt to solve based on starting and ending parameters
		if (solved(board, 14))
		{	
			//If puzzle has been solved
		
			//Print every move and board
			for (int i = 0; i < history.size(); i++)
			{
				System.out.println(moveHist.get(i));
				print(history.get(i));
			}	
		}
		else
		{
			//If puzzle has not been solved
		
			System.out.println("No solution is possible.");
		}
		
	}
	
	//Function to print board in a graphical way
	static void print(char[] board)
	{
        	System.out.println("    " + board[1]);
        	System.out.println("   " + board[2] + " " + board[3]);
        	System.out.println("  " + board[4] + " " + board[5] + " " + board[6]);
        	System.out.println(" " + board[7] + " " + board[8] + " " + board[9] + " " + board[10]);
        	System.out.println(board[11] + " " + board[12] + " " + board[13] + " " + board[14] + " " + board[15]);		
	}
	
	//function to copy board state
	static char[] copy(char[] board)
	{
		char[] copy = new char[board.length];
		
		for (int i = 0; i < board.length; i++)
		{
			copy[i] = board[i];
		}
		
		return copy;
	}
	
	//Function to solve puzzle peg game with backtracking
	static boolean solved(char[] board, int count)
	{
		//Iterate through moves
		for (int i = 0; i < moves.length; i++)
		{
		
			if (board[moves[i][0]] == 'o' && board[moves[i][1]] == 'o' && board[moves[i][2]] == '.')
			{
				//If a move can be made, make that move and decrement number of pegs remaining			
				board[moves[i][0]] = '.';
				board[moves[i][1]] = '.';
				board[moves[i][2]] = 'o';				
				count--;

				//After each move, copy board to board history, copy move to move history
				history.add(copy(board));
				moveHist.add("Moved " + moves[i][0] + " to " + moves[i][2] + ", jumping over " + moves[i][1]);
				
				//Recursive call that creates backtracking
				//This will be called until no more moves are possible
				if (solved(board, count))
				{
					//If this point is reached then the puzzle is solved
					
					//Return true as each recursive call is popped
					return true;
				}
				
				//If this point is reached then the puzzle is not solved and no more moves are possible
				//Dead end, must backtrack
				
				//Backtrack the last move, remove that board from board history, and remove that move from move history
				board[moves[i][0]] = 'o';
				board[moves[i][1]] = 'o';
				board[moves[i][2]] = '.';
				count++;
				history.remove(history.size() - 1);
				moveHist.remove(moveHist.size() - 1);
			}
			
			
		}	
		
		//If no more moves are possible
		if (count == 1 && end == 16)
		{
			//If there is only 1 peg remaining and the ending position does not matter, the puzzle is solved
			return true;
		}
		else if (count == 1 && board[end] == 'o')
		{
			//If there is only 1 peg remaining and the ending peg is in the correct location, the puzzle is solved
			return true;
		}
		else
		{
			//The puzzle is not solved
			return false;
		}			
	}
	
	
  
}
