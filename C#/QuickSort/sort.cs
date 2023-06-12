using System;

namespace fun
{
	class Program
	{
		//Function to swap elements of array
		static void swap(int[] each, int i, int j)
		{
			int temp = each[i];
			
			each[i] = each[j];
			
			each[j] = temp;
		}
	
		//Hoare partition
		static int partition(int[] each, int l, int r)
		{
			//Use first element as pivot
			int p = each[l];
			
			//Initialize scanning indices
			int i = l;
			int j = r + 1;
			
			//Create partition
			//Repeat until i >= j
			do
			{
				//Repeat until each[i] >= p
				do
				{
					i++;
					
				} while ( i != r && each[i] < p);
				
				//Repeat until each[i] <= p
				do
				{
					j--;
					
				} while (each[j] > p);
				
				//Swap elements at scanning indices
				swap(each, i, j);
				
			
			} while (i < j);
			
			//Undo last swap
			swap(each, i, j);
			
			//Swap element at j with pivot
			swap(each, l, j);
			
			//Return split position
			return j;
		}	
	
		static void quickSort(int[] each, int l, int r)
		{
			if (l < r)
			{
				//create split position
				int s = partition(each, l, r);
				
				//recursive call for subarrays surrounding split position
				quickSort(each, l, s - 1);
				quickSort(each, s + 1, r);	
			}
		}
	
		static void Main(String[] args)
		{
			// get name of file to process
			Console.Write("Enter name of file containing integers: ");
			string fileName = Console.ReadLine();
			
			//Does the file exist?
			if (!System.IO.File.Exists(fileName))
			{
				Console.WriteLine("Unable to open file");
				return;
			}
			
			//Convert each string into an integer and store in "eachInt[]"
			string fileContents = System.IO.File.ReadAllText(fileName);			
			string[] eachString = fileContents.Split(new char[] { ' ', '\t', '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);			
			int[] eachInt = new int[eachString.Length];			
			for (int i = 0; i < eachString.Length; i++)
			{
				eachInt[i] = int.Parse(eachString[i]);
			}

			//Sort eachInt
			quickSort(eachInt, 0, eachInt.Length - 1);
			
			//Print eachInt
			for (int i = 0; i < eachInt.Length; i++)
			{
				Console.WriteLine(eachInt[i]);
			}
		}
	}
}
