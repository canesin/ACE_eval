#include <stdio.h>
#include "ace_eval.h"

#define NCARDS 7

// Derived from 'allfive.c' by Kevin Suffecool
// http://suffecool.net/poker/code/allfive.c
/*************************************************/
/*                                               */
/* This code tests an evaluator, by looping over */
/* all 2,598,960 possible five card hands, cal-  */
/* culating each hand's distinct value, and dis- */
/* playing the frequency count of each hand type */
/*                                               */
/* Kevin L. Suffecool, 2001                      */
/* suffecool@bigfoot.com                         */
/*                                               */
/*************************************************/
unsigned int eval_hand( Card *hand, int ncards );
void init_deck( Card *deck );


/* ****************************************
//evaluators under test must provide:
typedef Card;   // a card type.  

int eval_hand( hand, ncards );  

void init_deck( Card *deck ) //
//   This routine initializes the deck.  A deck of cards is
//   simply an array of Cards of length 52 (no jokers).  This
//   routine should populate the array 

int hand_rank(int eval_result);
// returns one of 9 possible values indicating rank

char* value_str[]; //when indexed by hand_rank, gives rank name
*/


//Adapters for minieval
void init_deck(Card* deck) {
  int i;
	for (i=0;i<52;i++) deck[i]=ACE_makecard(i);
}

Card eval_hand(Card* hand,int n) {
  int i;
  Card h[5]={0};
  for (i=0;i<n;i++) ACE_addcard(h,hand[i]);
  return ACE_evaluate(h);
}

void verify(int a,int b, int c, int d, int e, int f, int g, Card xr)
{
  Card ar;
  Card h[5]={0};
  ACE_addcard(h,ACE_makecard(a));
  ACE_addcard(h,ACE_makecard(b));
  ACE_addcard(h,ACE_makecard(c));
  ACE_addcard(h,ACE_makecard(d));
  ACE_addcard(h,ACE_makecard(e));
  ACE_addcard(h,ACE_makecard(f));
  ACE_addcard(h,ACE_makecard(g));
  ar = ACE_evaluate(h);
  if (ar==xr) { printf ("ok.."); } 
  else printf("Error: %x != %x\n",ar,xr);
}

#define hand_rank(r)       ((r)>>28)

static char *value_str[] = {
  "High Card",
  "One Pair",
  "Two Pair",
  "Three of a Kind",
  "Straight",
  "Flush",
  "Full House",
  "Four of a Kind",
  "",
  "Straight Flush"
};

// *** Test Code ***
enum cn {
  c2H, c3H, c4H, c5H, c6H, c7H, c8H, c9H, cTH, cJH, cQH, cKH, cAH,
  c2C, c3C, c4C, c5C, c6C, c7C, c8C, c9C, cTC, cJC, cQC, cKC, cAC,
  c2D, c3D, c4D, c5D, c6D, c7D, c8D, c9D, cTD, cJD, cQD, cKD, cAD,
  c2S, c3S, c4S, c5S, c6S, c7S, c8S, c9S, cTS, cJS, cQS, cKS, cAS
}

main()
{
  Card deck[52], hand[7]={0}, freq[10]={0};
  int a, b, c, d, e, j;
  int y=0,z=-1;
  Card i;
  
  /* first verify some hands */
  printf("%s\n", ACE_makecard(c2H)==0x00000041?"OK":"ERR");
  printf("%s\n", ACE_makecard(c3H)==0x00000101?"OK":"ERR");
  printf("%s\n", ACE_makecard(cKH)==0x10000001?"OK":"ERR");
  printf("%s\n", ACE_makecard(cAH)==0x40000001?"OK":"ERR");
  printf("%s\n", ACE_makecard(c2C)==0x00000042?"OK":"ERR");
  printf("%s\n", ACE_makecard(c3C)==0x00000102?"OK":"ERR");
  printf("%s\n", ACE_makecard(cKC)==0x10000002?"OK":"ERR");
  printf("%s\n", ACE_makecard(cAC)==0x40000002?"OK":"ERR");
  printf("%s\n", ACE_makecard(c2D)==0x00000044?"OK":"ERR");
  printf("%s\n", ACE_makecard(c3D)==0x00000104?"OK":"ERR");
  printf("%s\n", ACE_makecard(cKD)==0x10000004?"OK":"ERR");
  printf("%s\n", ACE_makecard(cAD)==0x40000004?"OK":"ERR");
  printf("%s\n", ACE_makecard(c2S)==0x00000048?"OK":"ERR");
  printf("%s\n", ACE_makecard(c3S)==0x00000108?"OK":"ERR");
  printf("%s\n", ACE_makecard(cKS)==0x10000008?"OK":"ERR");
  printf("%s\n", ACE_makecard(cAS)==0x40000008?"OK":"ERR");

  //a kqjt 9876 5432
  //no hand
  verify(c2H,c3H,c4H,c5H,cTD,cJD,cKD, 0<<28|0x0000<<13|0x0B0C); 
  //1pr
  verify(cKH,c3H,c4H,c5H,cAD,cJD,cKD, 1<<28|0x0800<<13|0x1208); 
  //2pr 
  verify(cKD,c3H,c3C,c5H,cAD,cJD,cKS, 2<<28|0x0802<<13|0x1000); 
  verify(cKD,c3H,c3C,c5H,c5S,c2D,cKS, 2<<28|0x0808<<13|0x0002); 
  //trip
  verify(c3D,c4D,c6H,c3H,c9S,c3C,cTD, 3<<28|0x0002<<13|0x0180);
  //straight
  verify(c3D,c4D,c9H,c5H,c2D,c3C,cAD, 4<<28|0x0008<<13|0x0000);
  verify(c3D,c4D,c6H,c5H,c2D,c3C,cAD, 4<<28|0x0010<<13|0x0000);
  verify(c3D,c4D,c6H,c5H,c2D,c7C,cAD, 4<<28|0x0020<<13|0x0000);
  verify(cTD,cJD,cKH,c5H,cQD,c3C,cAD, 4<<28|0x1000<<13|0x0000);
  //flush
  verify(c2H,c3H,c4H,c5H,cTD,cJH,cKD, 5<<28|0x020F<<13|0x0000);
  verify(cKH,c3H,c4H,c5H,cAH,cJD,cKD, 5<<28|0x180E<<13|0x0000);
  verify(cKD,c3H,c3D,c5D,cAD,cJD,cKS, 5<<28|0x1A0A<<13|0x0000); 
  verify(c3D,c4D,c6D,c3H,c9D,c3C,cTD, 5<<28|0x0196<<13|0x0000);
  verify(c3D,c4D,c6D,c3H,c9D,c2D,cTD, 5<<28|0x0196<<13|0x0000);
  verify(c3D,c4D,c6D,c7D,c9D,cJD,cTD, 5<<28|0x03B0<<13|0x0000);
  //fh
  verify(c3D,c4D,c3H,c4H,c9S,c3C,cTD, 6<<28|0x0002<<13|0x0004);
  verify(c4S,c4D,c6H,c4H,c3S,c3C,cTD, 6<<28|0x0004<<13|0x0002);
  verify(c3D,c9D,c9H,c3H,c9S,c3C,cTD, 6<<28|0x0080<<13|0x0002);
  
  //quad
  verify(c3D,c9D,c9H,c3H,c3S,c3C,cTD, 7<<28|0x0002<<13|0x0100);

  //straight flush low
  verify(cAH,c2H,c3H,c4H,c5H,c9D,cTD, 9<<28|0x0008<<13|0x0000); 
  verify(cAD,c2H,c3H,cJD,cKD,cQD,cTD, 9<<28|0x1000<<13|0x0000); 
  verify(c9D,c7D,c8D,cJD,cKD,cQD,cTD, 9<<28|0x0800<<13|0x0000); 
  printf("\n");

  // initialize the deck
  init_deck( deck );
  
#if NCARDS == 7
  // loop over every possible seven-card hand
  for(y=0;y<46;y++)
  {
	 hand[5] = deck[y];
#if NCARDS >= 6 
    for(z=y+1;z<47;z++)
    {
		hand[6] = deck[z];
#if NCARDS >= 5

		//loop over every possible 5 card hand
		for(a=z+1;a<48;a++)
		{
		  hand[0] = deck[a];
		  for(b=a+1;b<49;b++)
		  {
			 hand[1] = deck[b];
			 for(c=b+1;c<50;c++)
			 {
				hand[2] = deck[c];
				for(d=c+1;d<51;d++)
				{
				  hand[3] = deck[d];
				  for(e=d+1;e<52;e++)
				  {
					 hand[4] = deck[e];
				
					 i = eval_hand( hand, NCARDS );
					 j = hand_rank(i);
					 freq[j]++;
				  }
				}
			 }
		  }
		}
#endif
    }
#endif
  }
#endif
  for(i=0;i<=9;i++)
	 printf( "%15s: %8d\n", value_str[i], freq[i]);


}


/*
Expected Output
---------------
      High Card: 23294460
       One Pair: 58627800
       Two Pair: 31433400
Three of a Kind:  6461620
       Straight:  6180020
          Flush:  4047644
     Full House:  3473184
 Four of a Kind:   224848
 Straight Flush:    41584

see: https://en.wikipedia.org/wiki/Poker_probability#Frequency_of_7-card_poker_hands

*/
