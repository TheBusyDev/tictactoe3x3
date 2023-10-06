/*
TIC TAC TOE GAME
UPDATES: 
function 'PrintTableWithNumbers' changed
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LEN 3
#define SPOTS LEN*LEN
#define MAX_SCORE SPOTS+1

int tab[SPOTS]; // game board

void PrintTableWithNumbers (int);
void PrintTable ();
int ExtractTurn (); // +1 = maximising player turn, -1 = minimising player turn
int AImove (int);
int minimax (int, int, int, int);
int CheckWinner (); // this function returns 0 if there is no winner, 10 if the winner is the computer, -10 if you are the winner
void clear ();
int insert (int, int);
int max (int, int);
int min (int, int);

int main ()
{
	int i, count; // counters
	int winner, move, turn, error;
	char ans;

	do
	{
		clear ();
		printf ("\n\033[1;32mTIC TAC TOE %dx%d, 3.0 VERSION\033[0m\n", LEN, LEN);
		printf ("\n\033[1mThese are the input positions:\n");
		
		// initialize the table
		for (i=0; i<SPOTS; i++)
			tab[i] = 0;

		count=0; // move counter

		PrintTableWithNumbers (count);
		printf ("\nYou are 'X' and I am 'O'.\nYour move must be a number between 1 and %d.\nPress [ENTER] to start the game...\033[0m\n", SPOTS);
		getchar ();
		
		clear ();
		srand (time(NULL));
		turn=ExtractTurn ();

		do 
		{		
			if (turn == +1)
			{
				if (!count)
					printf ("\n\033[1mThat's my move!\n");

				insert (+1, AImove (count));
				count++;
			}

			if (turn == -1)
			{
				if (!count)
					printf ("\n\033[1mIt's your turn!\n");

				do 
				{
					printf ("\033[1m");
					PrintTableWithNumbers (count);
					PrintTable ();

					printf ("\n\n\nYour move?\033[0m ");
					scanf ("%d", &move);
					getchar ();
					clear ();

					if (move>=1 && move <= SPOTS)
					{
						if (insert (-1, move))
						{
							count++;
							error=0;
						}
						else
						{
							printf ("\nError: position %d already occupied!\n", move);
							error=1;
						}
					}
					else 
					{
						printf ("\nError: your move must be a number between 1 and %d!\n", SPOTS);
						error=1;
					}
				} while (error);
			}

			turn=turn*(-1);
			winner=CheckWinner ();

			if (count == SPOTS && !winner) // if there is no empty space and there is no winner
				winner = -2;

		} while (!winner);

		printf ("\033[1m");
		PrintTable ();

		printf ("\n\n\n\033[1;36m");

		if (winner == -2)
			printf ("No winner :(");
		else if (winner == +1)
			printf ("The machines are superior! I am the winner!!");
		else if (winner == -1)
			printf ("Oh no, you defeated me!!");

		do 
		{
			printf ("\n\033[1;36mDo you like another match? [y/n]\033[0m\n");
			ans=getchar ();
			getchar ();

			if (ans != 'y' && ans != 'n')
				printf ("\nError: answer not allowed...\n");
		} while (ans != 'y' && ans != 'n');

	} while (ans == 'y');
	
	return 0;
}

void PrintTableWithNumbers (int count)
{
	int i, j;

	printf ("\n");

	if (count)
		printf ("Available spots:\n\n");

	for (i=1; i <= SPOTS; i++)
	{
		if (tab[i-1])
			printf (" - |");
		else 
			printf (" %d |", i);

		if (! (i%LEN))
		{
			printf ("\n");

			for (j=0; j < LEN*4; j++)
				printf ("-");

			printf ("\n");
		}
	}
}

void PrintTable ()
{
	int i, j;

	printf ("\n");

	for (i=0; i<SPOTS; i++)
	{
		if (tab[i] == 1)
			printf (" O |");
		else if (tab[i] == -1)
			printf (" X |");
		else
			printf ("   |");

		if (! ((i+1)%LEN))
		{
			printf ("\n");

			for (j=0; j < LEN*4; j++)
				printf ("-");

			printf ("\n");
		}
	}
}

int ExtractTurn ()
{
	int turn;
	
	turn=rand()%2;

	if (!turn)
		turn=-1;

	return turn;
}

int AImove (int count)
{	
	if (!count) // first move
		return (rand()%(SPOTS) + 1);

	int i;
	int score, ai_move;
	int best_score = -MAX_SCORE-1;
	int alpha = -MAX_SCORE-1, beta = +MAX_SCORE+1;

	for (i=1; i <= SPOTS; i++)
		if (insert (+1, i))
		{
			score = minimax (0, alpha, beta, 0);
			tab[i-1]=0; // reset the spot occupied

			alpha = max (score, alpha);
			
			if (score > best_score)
			{
				best_score = score;
				ai_move = i;
			}
			
			if (best_score == MAX_SCORE) // AI will win in 1 move - no more searching needed
				return ai_move;
		}

	return ai_move;
}

int minimax (int depth, int alpha, int beta, int Maximising_Turn)
{
	int i;
	int score, best_score;
	int winner;

	winner=CheckWinner ();

	if (winner == +1)
		return +MAX_SCORE-depth;
	else if (winner == -1)
		return -MAX_SCORE+depth;

	if (Maximising_Turn)
	{
		best_score = -MAX_SCORE-1;

		for (i=1; i <= SPOTS; i++)
			if (insert (+1, i))
			{
				score = minimax (depth+1, alpha, beta, 0);
				tab[i-1]=0; // reset the spot occupied

				best_score = max (score, best_score);
				alpha = max (score, alpha);

				if (best_score == +MAX_SCORE-depth || alpha > beta) // maximising player will win in 1 move - no more searching needed + alpha-beta pruning
					return best_score;
			}
	}
	else
	{
		best_score = +MAX_SCORE+1;

		for (i=1; i <= SPOTS; i++)
			if (insert (-1, i))
			{
				score = minimax (depth+1, alpha, beta, 1);
				tab[i-1]=0; // reset the spot occupied

				best_score = min (score, best_score);
				beta = min (score, beta);

				if (best_score == -MAX_SCORE+depth || alpha > beta) // minimising player will win in 1 move - no more searching needed + alpha-beta pruning
					return best_score;
			}
	}

	if (best_score == -MAX_SCORE-1 || best_score == +MAX_SCORE+1) // implies that there is no winner and no empty space
		return 0;
	else
		return best_score;
}

int CheckWinner ()
{
	int i, j, win;

	// check rows
	for (i=0; i<SPOTS; i=i+LEN)
	{
		win=1;

		if (!tab[i])
			win=0;

		for (j=1; j<LEN && win; j++)
			if (tab[i] != tab [i+j])
				win=0;

		if (win)
			return tab[i];
	}

	// check coloumns
	for (i=0; i<LEN; i++)
	{
		win=1;

		if (!tab[i])
			win=0;

		for (j=LEN; j<SPOTS && win; j=j+LEN)
			if (tab[i] != tab [i+j])
				win=0;

		if (win)
			return tab[i];
	}
	
	// check diagonals
	// check first diagonal
	win=1;

	if (!tab[0])
		win=0;

	for (i = LEN+1; i < SPOTS && win; i = i + (LEN+1))
		if (tab[0] != tab[i])
			win=0;

	if (win)
		return tab[0];

	// check second diagonal
	win=1;

	if (!tab[LEN-1])
		return 0;

	for (i = (LEN-1)*2; i <= (LEN-1)*LEN && win; i = i + (LEN-1))
		if (tab[LEN-1] != tab[i])
			win=0;

	if (win)
		return tab[LEN-1];

	return 0;
}

void clear ()
{
	int i;

	for (i=0; i<60; i++)
		printf ("\n");
}

int insert (int player, int position)
{
	if (tab[position-1]) // position already occupied
		return 0;
	
	tab[position-1] = player;

	return 1;
}

int max (int a, int b)
{
	if (a>=b)
		return a;
	else
		return b;
}

int min (int a, int b)
{
	if (a<=b)
		return a;
	else
		return b;
}