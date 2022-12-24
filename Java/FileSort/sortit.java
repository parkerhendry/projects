//Parker Hendry
/*
This program will sort lines of strings lexicographically. The algorithm used is based on a version of mergesort. This program
will accept input files and standard input.
*/


import java.io.File;
import java.util.Arrays;
import java.util.Scanner;

//Create class to establish queues
class Queue
{
	//Encapsulate front, back, and size of queue
	private Node head;
	private Node tail;
	private int size;
	
	//Create class to establish nodes within queue
	class Node
	{
		//Encapsulate data and next pointer of each node
		private String data;
		private Node next;
		
		//Node constructor that accepts data for mutation
		Node(String d)
		{
			data = d;
		}
		
		//Accessor method for data within node
		public String getData()
		{
			return data;
		}
	}
	
	//Method for enqueueing data (adding to the back)
	public void enqueue(String data)
	{
		Node newNode = new Node(data);
		
		if (this.head == null)
		{
			this.head = newNode;
			this.tail = newNode;
		}
		else
		{		
			Node bar = this.tail;		
			bar.next = newNode;			
			this.tail = newNode;		
		}
		
		this.size++;
	}
	
	//Method for dequeueing data (removing from the front)
	public String dequeue()
	{
		String answer = this.head.getData();
		
		Node bar = this.head;
		
		bar = bar.next;
		
		this.head = bar;
		
		this.size--;
		
		return answer;
	}
	
	//Method for printing entire queue
	public void print()
	{
		Node bar = this.head;
		
		while (bar.next != null)
		{			
			System.out.println(bar.getData());
			bar = bar.next;
		}
		
		System.out.println(bar.getData());
		
		
	}
	
	//Method for returning the data in the front of the queue
	public String peekFront()
	{
		return this.head.getData();
	}

	//Method for returning the data at the back of the queue	
	public String peekBack()
	{
		return this.tail.getData();
	}
	
	//Boolean method for testing if empty or not
	public boolean isEmpty()
	{
		return (this.head == null) ? true : false;
	}
	
	//Method for returning the size of the queue
	public int size()
	{
		return this.size;
	}
					
}


public class sortit {

	//Method for accepting user input
	static String[] standard(String[] lines)
	{
	
		Scanner scan = new Scanner(System.in);
		
		String line;
		
		boolean next = true;
		
		//Accept user input from user until nothing is entered
		while (next)
		{		
			try
			{
				line = scan.nextLine();
		
				lines = Arrays.copyOf(lines, lines.length + 1);
				
				lines[lines.length - 1] = line;
			}
			catch (java.util.NoSuchElementException e)
			{
				next = false;
			}
		}
		
		return lines;
	}
	
	//Method for accpeting input files
	static String[] file(String fileName, String[] lines) throws Exception
	{
	
		File foo = new File(fileName);
		
		//If file does not exist, print error
		if (!foo.exists())
		{
			System.err.println("ERROR: File " + fileName + " not found.");
			return lines;
		}
		
		Scanner bar = new Scanner(foo);
		
		String line;
		
		//Read file until end
		while (bar.hasNextLine())
		{
			line = bar.nextLine();
			
			lines = Arrays.copyOf(lines, lines.length + 1);
			
			lines[lines.length - 1] = line;
		}
		
		return lines;
			
	}

	//Main method
	public static void main(String[] args) throws Exception
	{
	
		//If user entered no command line arguments, exit
		if (args.length < 1)
		{
			System.out.println("Please enter a file as a command line argument.");
			return;
		}
	
		int j = 0;	
		
		String[] lines = new String[0];	
	
		//Combine all lines from command line arguments
		while (j < args.length)
		{
			
			//If the user entered "-" call standard function
			//If the user entered a file, call file function
			//Combine lines into lines array
			if (args[j].equals("-"))
			{
				lines = standard(lines);	
			}
			else
			{
				lines = file(args[j], lines);	
			}	
			
			j++;					
		}
		
		//If no lines to sort, exit
		if (lines.length < 1)
		{
			return;
		}
		
		//Create 2 queues
		Queue list0 = new Queue();
		Queue list1 = new Queue();
			
		boolean change = true;
		
		//Add lines to queues
		//When a line is encountered that's less than the previous, switch lists
		for (int i = 0; i < lines.length; i++)
		{
			
			if (i == 0)
			{
				list0.enqueue(lines[i]);
			}
			else if (lines[i].compareTo(lines[i - 1]) < 0 && change == true)
			{
				list1.enqueue(lines[i]);
				
				change = false;
			}
			else if (lines[i].compareTo(lines[i - 1]) < 0 && change == false)
			{
				list0.enqueue(lines[i]);
				
				change = true;
			}
			else if (change == true)
			{
					
				list0.enqueue(lines[i]);	
					
			}
			else if (change == false)
			{
				list1.enqueue(lines[i]);
			}	
		}
		
		
		//Begin sorting
		//Reptition structure for passes
		//Repeat while all elements are not on a single list
		while (!(list0.isEmpty()) && !(list1.isEmpty()))
		{

			//Limit variables to mark queue
			//These are used like grocery store dividers
			int limit1 = list0.size();
			int limit2 = list1.size();
			
			boolean baz = false;
			
			//Variables to hold last enqueued
			String list0Last = "";
			String list1Last = "";
			
			//While neither list is exhausted
			while ((limit1 > 0) && (limit2 > 0))
			{				
				//While adding to first list
				while ((baz == false) && ((limit1 > 0) && (limit2 > 0)))
				{
					if ((list0.peekFront()).compareTo(list0Last) >= 0 && (list1.peekFront()).compareTo(list0Last) >= 0)
					{	
						//If both head elements are greater than the last enqueued, choose the smaller
									
						if ((list0.peekFront()).compareTo(list1.peekFront()) < 0)
						{	
							list0.enqueue(list0.dequeue());
							list0Last = list0.peekBack();
							limit1--;
						}	
						else
						{
							list0.enqueue(list1.dequeue());
							list0Last = list0.peekBack();
							limit2--;							
						}													
					}
					else if ((list0.peekFront()).compareTo(list0Last) >= 0)
					{
						//If head element from list0 is greater than the last enqueued					
					
						list0.enqueue(list0.dequeue());
						list0Last = list0.peekBack();
						limit1--;
	
					}
					else if ((list1.peekFront()).compareTo(list0Last) >= 0)
					{
						//if head element from list1 is greater than the last enqueued
					
						list0.enqueue(list1.dequeue());
						list0Last = list0.peekBack();
						limit2--;					
					}
					else 
					{
						//If neither head elements are greater than the last enqueued, switch lists
						
						baz = true;
						list1Last = "";
						
					}
					
					
				}
				
				//While adding to second list
				while ((baz == true) && ((limit1 > 0) && (limit2 > 0)))
				{
				
				
					if ((list0.peekFront()).compareTo(list1Last) >= 0 && (list1.peekFront()).compareTo(list1Last) >= 0)
					{
						//If both head elements are greater than the last enqueued, choose the smaller
					
						if ((list0.peekFront()).compareTo(list1.peekFront()) < 0)
						{
							list1.enqueue(list0.dequeue());
							list1Last = list1.peekBack();
							limit1--;
						}	
						else
						{
							list1.enqueue(list1.dequeue());
							list1Last = list1.peekBack();
							limit2--;
						}
											
					}
					else if ((list0.peekFront()).compareTo(list1Last) >= 0)
					{
						//If head element from list0 is greater than the last enqueued
					
						list1.enqueue(list0.dequeue());
						list1Last = list1.peekBack();
						limit1--;
						
	
					}
					else if ((list1.peekFront()).compareTo(list1Last) >= 0)
					{
						//if head element from list1 is greater than the last enqueued
					
						list1.enqueue(list1.dequeue());
						list1Last = list1.peekBack();
						limit2--;
						
					
					}
					else 
					{
						//If neither head elements are greater than the last enqueued, switch lists
					
						baz = false;
						list0Last = "";
					
					}					
				}
				
			}		
			
			//While list0 is not exhausted
			while (limit1 > 0)
			{
				//While adding to the first list
				while (baz == false && limit1 > 0)
				{
					if ((list0.peekFront()).compareTo(list0Last) >= 0)
					{
						//If head element from list0 is greater than the last enqueued
					
						list0.enqueue(list0.dequeue());
						list0Last = list0.peekBack();
						limit1--;
					}
					else
					{
						//If head element from list0 is not greater than the last enqueued, switch lists
					
						baz = true;
						list1Last = "";	
					}
					
					
				}
				
				//While adding to the second list
				while (baz == true && limit1 > 0)
				{
					if ((list0.peekFront()).compareTo(list1Last) >= 0)
					{
						//If head element from list0 is greater than the last enqueued
					
						list1.enqueue(list0.dequeue());
						list1Last = list1.peekBack();
						limit1--;
					}
					else
					{
						//If head element from list0 is not greater than the last enqueued, switch lists
					
						baz = false;	
						list0Last = "";
					}					
				}
			}
			
			//While list1 is not exhausted		
			while (limit2 > 0)
			{
				//While adding to the first list
				while (baz == false && limit2 > 0)
				{
					if (list1.peekFront().compareTo(list0Last) >= 0)
					{
						//If head element from list1 is greater than the last enqueued
					
						list0.enqueue(list1.dequeue());
						list0Last = list0.peekBack();
						limit2--;
					}
					else
					{
						//If head element from list1 is not greater than the last enqueued, switch lists
					
						baz = true;
						list1Last = "";	
					}
				}
				
				//While adding to the second list
				while (baz == true && limit2 > 0)
				{
					if (list1.peekFront().compareTo(list1Last) >= 0)
					{
						//If head element from list1 is greater than the last enqueued
					
						list1.enqueue(list1.dequeue());
						list1Last = list1.peekBack();
						limit2--;
					}
					else
					{
						//If head element from list1 is not greater than the last enqueued, switch lists
					
						baz = false;	
						list0Last = "";
					}					
				}
			}
				
			
		}
		
		
		//Print the non-empty queue 
		if (!(list0.isEmpty()))
		{
			list0.print();
		}	
		
		if (!(list1.isEmpty()))
		{
			list1.print();
		}
		
				
		
	}
  
}
