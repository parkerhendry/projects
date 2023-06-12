//Parker Hendry
/*
This program will accept either one or two command line arguments. The first command line argument accepts a word search grid and the second accepts a list of words to search for. If the second command line argument is absent then the user is prompted for a word to search for.
*/


import java.util.*;
import java.io.File;

class WordFind
{	

	//Function to search grid for words
	static boolean gridSearch(char[][] array, int i, int k, String word, int count, String origWord)
	{
	
		//Array for horizontal search direction
		int[] rowDir = {-1, -1, -1, 0, 0, 1, 1, 1};
		
		//Array for vertical search direction
		int[] colDir = {-1, 0, 1, -1, 1, -1, 0, 1}; 			
	
		//Loop to search all 8 directions
		for (int j = 0; j < 8; j++)
		{
			//Create value to hold length of matching elements
			int len;
			
			//Add value to row number to establish direction
			int foo = i + rowDir[j];
			
			//Add value to column number to establish direction
			int bar = k + colDir[j];
				
			//Search in the same direction until a non-match is found or the search goes off the grid
			//If the length of the word is exhausted, the word is found		
			for (len = 1; len < word.length(); len++)
			{
				
				//If search in a given direction goes off the grid, break
				if (foo >= array.length || foo < 0 || bar >= array[0].length || bar < 0)
					break;
					
				//Increment character comparison counter
				count++;
			
				//If element in grid does not equal corresponding element in word, break
				if (array[foo][bar] != word.charAt(len))
					break;
				
				//Since element in grid equals corresponding element in word, continue in the same direction	
				foo += rowDir[j];
				bar += colDir[j];
			}
			
			//If word is found, print location, direction, and character comparison count
			//Return true value
			if (len == word.length())
			{
			
				
				if (rowDir[j] == 1 && colDir[j] == 0)
					System.out.println(origWord + " was found starting at " + (i + 1) + "," + (k + 1) + " and oriented South with " + count + " character comparisons.");
				else if (rowDir[j] == 0 && colDir[j] == 1)
					System.out.println(origWord + " was found starting at " + (i + 1) + "," + (k + 1) + " and oriented East with " + count + " character comparisons.");
				else if (rowDir[j] == -1 && colDir[j] == 0)
					System.out.println(origWord + " was found starting at " + (i + 1) + "," + (k + 1) + " and oriented North with " + count + " character comparisons.");
				else if (rowDir[j] == 0 && colDir[j] == -1)	
					System.out.println(origWord + " was found starting at " + (i + 1) + "," + (k + 1) + " and oriented North with " + count + " character comparisons.");
				else if (rowDir[j] == 1 && colDir[j] == 1)
					System.out.println(origWord + " was found starting at " + (i + 1) + "," + (k + 1) + " and oriented Southeast with " + count + " character comparisons.");
				else if (rowDir[j] == 1 && colDir[j] == -1)
					System.out.println(origWord + " was found starting at " + (i + 1) + "," + (k + 1) + " and oriented Southwest with " + count + " character comparisons.");	
				else if (rowDir[j] == -1 && colDir[j] == 1)
					System.out.println(origWord + " was found starting at " + (i + 1) + "," + (k + 1) + " and oriented Northeast with " + count + " character comparisons.");
				else if (rowDir[j] == -1 && colDir[j] == -1)	
					System.out.println(origWord + " was found starting at " + (i + 1) + "," + (k + 1) + " and oriented Northwest with " + count + " character comparisons.");
					
				return true;
			}
			
		}
		
		//If word is not found, return false
		return false;
		
				
	}
	
	//Function to move grid from file to 2-D char array
	static char[][] getGrid(String filename) throws Exception
	{
	
		//string to hold line
		String line;
	
		int num = 1, j = 0;
	
		//2-D character array to hold grid
		char[][] wordGrid = new char[0][];
		
		//Create file object for grid file	
		File foo = new File(filename);
		
		//Create scanner object to read from file	
		Scanner bar = new Scanner(foo);
		
		//Loop through each line in file	
		while (bar.hasNextLine())
		{
			//Input line from file
			line = bar.nextLine();
			
			//Remove all non-alphanumeric characters
			line = line.replaceAll("[^a-zA-Z0-9]", "");
			
			//Input lines that contain letters 	
			if (num % 2 == 0)
			{
				//Move 2-D array to larger 2-D array
				wordGrid = Arrays.copyOf(wordGrid, wordGrid.length + 1);
					
				//Convert line string to char array
				wordGrid[j] = line.toCharArray();
					
				j++;
				
				
			}
				
			num++;
		}
		
		//Return 2-D char array containing word search grid	
		return wordGrid;
		
				
	}
	
	static void traverseGrid(char[][] wordGrid, String word)
	{
		//Convert word to all uppercase
		word = word.toUpperCase();
		
		String origWord = word;
	
		//Remove spaces from word
		word = word.replaceAll("\\s", "");
				
		//Boolean value to hold if found or not
		boolean answer = false;
				
		//Character comparison counter
		int count = 0;
				
		//Traverse word grid		
		for (int i = 0; i < wordGrid.length; i++)
		{
			for (int k = 0; k < wordGrid[i].length; k++)
			{
					
				//If element in word grid matches first element in word string
				if (wordGrid[i][k] == word.charAt(0))
				{
					//Call gridSearch() function and place result in answer
					answer = gridSearch(wordGrid, i, k, word, count, origWord);
				
				}
					
				//Increment character comparison counter
				count++;
					
				//If word is found, break
				if (answer)
					break;
					
					
			}
				
			//If word is found, break
			if (answer)
				break;
				
		}
				
		//If word was not found, print
		if (!answer)
			System.out.println(origWord + " was not found.");
	
		
	}
	
	public static void main(String[] args) throws Exception
	{
		if (args.length < 1)
		{
			//If no command line arguments, print error and return
		
			System.out.println("Please enter a word find grid as a command line argument.");
			return;
		}
		else if (args.length > 2)
		{
		
			//If too many command line arguments, print error and return
		
			System.out.println("Please enter only one or two command line arguments.");
			return;
		}
		else if (args.length == 1)
		{
		
			//If one command line argument, prompt user for word
		
			String word;
			
			//Create scanner object to hold user input
			Scanner scan = new Scanner(System.in);
			
			
			word = "foo";
			
			//Call getGrid() function and store word grid in 2-D array wordGrid
			char[][] wordGrid = getGrid(args[0]);	
				
			while (word.length() >= 1)
			{
			
				System.out.println("What word would you like to search for?");
			
				//Input word from user
				word = scan.nextLine();
			
				//If no word entered, return
				if (word.length() < 1)
					return;			
				
				traverseGrid(wordGrid, word);
										
			}
			
			
						
		}
		else if (args.length == 2)
		{
		
			//If two command line arguments, read words from file
		
			//Create string to hold each word
			String word;
			
			//Create file object for words file
			File words = new File(args[1]);
			
			//Create scanner object to read words file
			Scanner scan = new Scanner(words);
			
			//Call getGrid() function and store word grid in 2-D array wordGrid
			char[][] wordGrid = getGrid(args[0]);
			
			//Read each word from file and search for it in grid
			while (scan.hasNextLine())
			{	
				//Read word from file
				word = scan.nextLine();
				
				//If no word then return
				if (word.length() < 1)
					return;				
			
				traverseGrid(wordGrid, word);
			}
			
			
		}
	}
}
