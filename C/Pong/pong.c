//Parker Hendry
//2023

#include <ncurses.h>
#include <stdlib.h>

struct Paddle
{
	int x;
	int y;
	struct Paddle *next;
};

struct Paddle* initRight()
{
	struct Paddle *right1 = malloc(sizeof(*right1));
	struct Paddle *right2 = malloc(sizeof(*right2));
	struct Paddle *right3 = malloc(sizeof(*right3));
	struct Paddle *right4 = malloc(sizeof(*right4));	
	
	right1->x = COLS - 10;
	right1->y = 30;
	right1->next = right2;
	right2->x = COLS - 10;
	right2->y = 31;
	right2->next = right3;
	right3->x = COLS - 10;
	right3->y = 32;
	right3->next = right4;
	right4->x = COLS - 10;
	right4->y = 33;
	right4->next = NULL;
	
	return right1;	
}  

struct Paddle* initLeft()
{
	struct Paddle *left1 = malloc(sizeof(*left1));
	struct Paddle *left2 = malloc(sizeof(*left2));
	struct Paddle *left3 = malloc(sizeof(*left3));
	struct Paddle *left4 = malloc(sizeof(*left4));	
	
	left1->x = 10;
	left1->y = 30;
	left1->next = left2;
	left2->x = 10;
	left2->y = 31;
	left2->next = left3;
	left3->x = 10;
	left3->y = 32;
	left3->next = left4;
	left4->x = 10;
	left4->y = 33;
	left4->next = NULL;
	
	return left1;	
} 

void moveUp(struct Paddle *bar)
{
	int test = bar->y - 1;

	if (test < 0)
		return;
	else
	{

		while (bar)
		{
			bar->y -= 1;
			bar = bar->next;
		}
	
	}	
}

void moveDown(struct Paddle *bar)
{
	int test = bar->y + 4;
	
	if (test == LINES)
		return;
	else
	{
		while (bar)
		{
			bar->y += 1;
			bar = bar->next;
		}
	}
}

int main()
{
	initscr();
	keypad(stdscr, TRUE);
	cbreak();
	noecho();
	curs_set(0);
	timeout(100);
	
	clear();
	
	int tempX = 1;
	int tempY = 1;
	
	struct Paddle *left = initLeft();
	struct Paddle *right = initRight();
	
	int fredX = 1;
	int fredY = 1;
	
	while (1)
	{
		clear();
		
		if (tempY == LINES - 1)
			fredY = -1;
		else if (tempY == 0)
			fredY = 1;
		else if (tempX == left->x - 10 || tempX == right->x + 10)
			break;
				
		struct Paddle *current = left;
		int counter = 0;
		
		while (current)
		{
			mvaddch(current->y, current->x, ACS_BLOCK);
						
			if ((current->y == tempY && current->x == tempX) && counter < 2)
			{
				fredY = -1;
				fredX = 1;
			}
			
			if ((current->y == tempY && current->x == tempX) && counter >= 2)
			{
				fredY = 1;
				fredX = 1;
			}
						
			current = current->next;
			
			counter++;
		}
		
		current = right;
		counter = 0;
		
		while (current)
		{
			mvaddch(current->y, current->x, ACS_BLOCK);
						
			if ((current->y == tempY && current->x == tempX) && counter < 2)
			{
				fredY = -1;
				fredX = -1;
			}
			
			if ((current->y == tempY && current->x == tempX) && counter >= 2)
			{
				fredY = 1;
				fredX = -1;
			}
			
			current = current->next;
			
			counter++;
		}
		
		tempY += fredY;
		tempX += fredX;
		
		mvaddch(tempY, tempX, ACS_DIAMOND);
		
		refresh();
		
		int ch = getch();
		
		switch(ch)
		{
			case KEY_UP:
				moveUp(right);	
				break;
			case KEY_DOWN: 
				moveDown(right);
				break;
			case 'W':
			case 'w':
				moveUp(left);
				break;
			case 'S':
			case 's':
				moveDown(left);
				break;
			default:
				break;
		}
		
		
	}
	
	endwin();
	
	
}
