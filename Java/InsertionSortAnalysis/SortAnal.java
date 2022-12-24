import java.util.Random;	//used to generate random arrays
import java.util.Arrays;	//used to sort arrays

//public class that can be used in other packages
public class SortAnal
{
	//algorithm implementation
	static int sortAnal(int[] array)
	{
		int count = 0;
		int v,j;
		
		//Iterate through array
		for (int i = 1; i <= array.length - 1; i++)
		{
			v = array[i];
			
			j = i - 1;
			
			//Find which elements are greater than array[i] and insert them into correct spots
			while (j >= 0 && array[j] > v)
			{
				count++;			//Count A[j] > v operations
				array[j + 1] = array[j];
				j = j - 1;
			}
			
			//Account for j ending the while loop instead of A[j] > v
			if (j >= 0)
				count++;
				
			//Insert array[i] into correct spot
			array[j + 1] = v;
			
		}	
		
		//Return basic operation count
		return count;
	}

	//function used to reverse sorted arrays to create arrays sorted by descending order
	static int[] reverse(int[] array, int n)
	{
		//Create array to hold reversal
		int[] foo = new int[n];
		
		//j = size
		int j = n;
		
		//Loop through array, assigning mirrored positions
		for (int i = 0; i < n; i++)
		{
			foo[j - 1] = array[i];
			j--;
		}
		
		//Return reversed array
		return foo;
	}

	//main function
	public static void main(String[] args)
	{
		//Create rand object from Random class
		Random rand = new Random();
		
		//Create format for printing results
		//This says create 3 fields, each with 20 spaces, and a newline at the end
		String format = "%-20s%-20s%-20s%n";	
	
		//Create variable to hold size	
		int j = 1000;
		
		//Create variable to hold efficiency evaluation for comparison
		int k;
		
		//Create 20 arrays
		int[][] arrays = new int[20][];

		//Header
		System.out.println("Arrays 1-20 counts - UNSORTED");

		//Column headers		
		System.out.printf(format,"n:", "Count:", "(n^2/4):");
		
		//Loop for 20 unsorted arrays
		for (int i = 0; i < 20; i++)
		{
		
			//Generate size j stream of random integers and turn them into an array
			arrays[i] = rand.ints(j).toArray();
			
			//Create n^2/4 evaluation for comparison 
			k = (int)Math.pow(j, 2) / 4;
			
			//Print size, basic operation count, and n^2/4 evaluation
			System.out.printf(format, j, sortAnal(arrays[i]), k);
			
			//Increase size
			j = j + 1000;
		}
		
		//Print empty line for cleanliness
		System.out.println();
		
		//Reset size
		j = 1000;
		
		//Header
		System.out.println("Arrays 1-20 counts - SORTED (ASCENDING)");
		
		//Column headers
		System.out.printf(format,"n:", "Count:", "(n-1):");
		
		//Loop for 20 sorted (ascending) arrays
		for (int i = 0; i < 20; i++)
		{
			//Generate size j stream of random integers and turn them into an array
			arrays[i] = rand.ints(j).toArray();
		
			//Sort array with sort method from Arrays class
			Arrays.sort(arrays[i]);
			
			//Print size, basic operation count, and n - 1 evaluation			
			System.out.printf(format, j, sortAnal(arrays[i]), (j - 1));
			
			//Increase size
			j = j + 1000;	
		}
		
		//Print empty line
		System.out.println();
		
		//Reset size
		j = 1000;
		
		//Header
		System.out.println("Arrays 1-20 counts - SORTED (DESCENDING)");
		
		//Column headers
		System.out.printf(format,"n:", "Count:", "(n*(n-1)/2):");

		//Loop for 20 sorted (descending) arrays		
		for (int i = 0; i < 20; i++)
		{
			//Generate size j stream of random integers and turn them into an array
			arrays[i] = rand.ints(j).toArray();

			//Sort array with sort method from Arrays class		
			Arrays.sort(arrays[i]);
			
			//Reverse array, producing an array sorted by descending order
			arrays[i] = reverse(arrays[i], j);
			
			//Create n*(n-1)/2 evaluation for comparison
			k = (j * (j - 1)) / 2;
			
			//Print size, basic operation count, and n*(n-1)/2 evaluation
			System.out.printf(format, j, sortAnal(arrays[i]), k);
			
			//Increase size
			j = j + 1000;	
		}		
		
		
		
		
				
	}
}
