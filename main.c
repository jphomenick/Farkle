// Farkle
// This program allows multiple users to play a dice game named Farkle.
// Rules of the game: https://www.dicegamedepot.com/farkle-rules/
// Number of players: 2 to 4
// Joseph Homenick

#include <stdio.h>


// functions and variables for random number generation
void seedRandom(unsigned int seed);  // initialize random number generator
unsigned int getRandom(unsigned int limit);  // returns random integer between 0 and limit-1

// dice data structure: an array of seven values
// index 0: total number of dice in the collection
// index 1-6: element i is the number of dice with face value = i
typedef int DiceGroup[7];
// if you don't understand how typedef works, or just prefer to not use this type,
// just declare an array of 7 integers instead -- you will not lose points for this

int takeTurn();   // interact with user to roll dice and return score
void rollDice(DiceGroup, int);     // initialize a specified number of dice
void printDice(const DiceGroup);   // print dice as a sorted sequence of digits
int selectDice(DiceGroup dice, DiceGroup keep, int choice);   // move dice from initial set to "keep" set
int scoreDice(const DiceGroup);    // return highest score available for a set of dice
int setDice(DiceGroup, int);       // initialize dice using an integer representation
int testFarkle(const DiceGroup);   // return 1 if no scoring dice

// main function: user interface
int main() {
    int players = 0;  // number of players -- must be 2 to 4
    int scores[4] = {0};  // player scores -- start at zero
    int winner = 0;       // winning player (zero until someone wins)
    int p = 0;            // current player
    int i;        // used for loops
    unsigned int seed;    // initializer for random number generator

    setbuf(stdout,0);
    printf("Welcome to Farkle!\nHow many players? ");
    scanf("%d", &players);
    if ((players < 1) || (players > 4)) {
        printf("This game only supports 1 to 4 players. How many? ");
        scanf("%d", &players);
    }
    printf("Enter a seed integer (decimal or hexadecimal): ");
    scanf("%i", &seed);
    seedRandom(seed);

    while (!winner) {
        int score = 0;
        printf("\nSCORES -- ");
        for (i=0; i<players; i++) printf("%d: %d  ", i+1, scores[i]);
        printf("\nPlayer %d's turn\n", p+1);
        score = takeTurn();
        printf("Turn score = %d\n", score);
        if (players == 1) return 0;    // 1 player means one turn (used for testing)
        scores[p] += score;
        if (scores[p] >= 10000) winner = p+1;
        p = (p + 1) % players;
    }
    printf("\nSCORES -- ");
    for (i=0; i<players; i++) printf("%d: %d  ", i+1, scores[i]);
    printf("\nPlayer %d wins!", winner);
    return 0;
}


// printDice Function: This function prints the dice as a sequence of decimal digits.
void printDice(const DiceGroup dice) {

    int value = 0;
    for (int i = 1; i < 7; i++) {             //i corresponds to the face value of the dice (i.e. 1, 2, 3, ... 6)
        value = dice[i];                      //value is the number of dice with face value, i

        for (int j = 0; j < value; j++) {     //prints the dice as digits in sorted order
            printf("%d", i);
        }
    }
}

//rollDice Function: Simulates a roll of n number of dice by getting n number of random numbers between 1 and 6.
//The values of the rolls are used to initialize the dice array according to the DiceGroup data structure.
void rollDice(DiceGroup dice, int n) {
    int num = 0;
    int count1 = 0;
    int count2 = 0;
    int count3 = 0;
    int count4 = 0;
    int count5 = 0;
    int count6 = 0;

    for (int i = 0; i < n; i++) {               //Loop counts the occurrence of each dice face value during the n rolls
        num = (getRandom(6)) + 1;
        if (num == 1) count1++;
        if (num == 2) count2++;
        if (num == 3) count3++;
        if (num == 4) count4++;
        if (num == 5) count5++;
        if (num == 6) count6++;
    }

    dice[0] = n;                               //Occurrence of each dice value placed in corresponding array elements
    dice[1] = count1;                          //according to the dice data structure. Note: element zero is number
    dice[2] = count2;                          //dice rolled (variable n)
    dice[3] = count3;
    dice[4] = count4;
    dice[5] = count5;
    dice[6] = count6;

}
//setDice Function: Returns 0 if any there is a value in data which cannot be on the face of a die. Otherwise, the dice
//values used to initialize the dice array according to the data structure, and the return value is 1.
int setDice(DiceGroup dice, int data) {
    int retValue = 1;                          //initialize return value to 1
    int num = 0;
    int digits = 0;                            //number of digits in data
    int base = 1;                              //base which will be used for integer division
    int isolated = 0;                          //isolated digit from data
    int count1 = 0;                            //counters for die values (1-6)
    int count2 = 0;
    int count3 = 0;
    int count4 = 0;
    int count5 = 0;
    int count6 = 0;

    num = data;
    while (num > 0) {                          //loop calculates number of digits in data
        digits++;
        num /= 10;
    }

    //Outer loop starts at rightmost digit and isolates each digit
    num = data;
    for (int i = digits; i > 0; i--) {
        for (int exponent = (i - 1); exponent > 0; exponent--) {    //Inner loop calculates a power of 10 which
            base *= 10;                                             //will be used in integer division/modulo for
        }                                                           //isolating digits.
        isolated = (num / base) % 10;

        if ((isolated == 0) || (isolated > 6)) {                    //If isolated digit is invalid (zero or 7,8,9)
            retValue = 0;                                           //return value is zero; exit loop.
            break;
        }
        base = 1;                                                    //reset base for next loop iteration

        if (isolated == 1) count1++;                                 //count instances of each dice value (1-6)
        if (isolated == 2) count2++;
        if (isolated == 3) count3++;
        if (isolated == 4) count4++;
        if (isolated == 5) count5++;
        if (isolated == 6) count6++;
    }

    dice[0] = digits;                           //Occurrence of each dice value placed in corresponding array elements
    dice[1] = count1;                           //according to the dice data structure. Note: element zero is number of
    dice[2] = count2;                           //dice (variable 'digits')
    dice[3] = count3;
    dice[4] = count4;
    dice[5] = count5;
    dice[6] = count6;


    return retValue;
}

//testFarkle Function: Returns 1 if the dice array has no scoring dice. Otherwise, returns 0.
int testFarkle(const DiceGroup dice) {
    int retValue = 1;                           //Initialize return value to one
    int straight = 1;                           //Initialize straight value to 1 (true)

    if (dice[1] >= 1) retValue = 0;             //Testing for at least one 1
    if (dice[5] >= 1) retValue = 0;             //Testing for at least one 5

    if (dice[2] >= 3) retValue = 0;             //Testing for at least three 2's
    if (dice[3] >= 3) retValue = 0;             //Testing for at least three 3's
    if (dice[4] >= 3) retValue = 0;             //Testing for at least three 4's
    if (dice[6] >= 3) retValue = 0;             //Testing for at least three 6's

    for (int i = 1; i < 7; i++) {               //Testing for straight
        if ((dice [0] != 6) || (dice[i] != 1)) {
            straight = 0;
        }
    }

    if (straight == 1) retValue = 0;

    return retValue;

}

//scoreDice: Returns the highest possible score based on the dice array. All scoring dice are counted.
int scoreDice(const DiceGroup dice) {
    int scoreSum = 0;                           //Initialize score to zero.
    int straight = 1;                           //Initialize straight value to 1 (true)

//If there are at least 3 dice with the value 1, dice[1] / 3 will give the number of groups of three ones (1000pts for
//each group) and dice[1] % 3 will give the remaining individual number of ones (100pts for each).
    if (dice[1] >= 3) {
        scoreSum += (dice[1] / 3) * 1000;
        scoreSum += (dice[1] % 3) * 100;
    }
    else scoreSum += (dice[1]) * 100;       //If there are less than 3 dice with value 1, count individual ones (100pts
                                            //for each).

//If there are at least 3 dice with the value 5, dice[5] / 3 will give the number of groups of three fives (500pts for
//each group) and dice[5] % 3 will give the remaining individual number of fives (500pts for each).
    if (dice[5] >= 3) {
        scoreSum += (dice[5] / 3) * 500;
        scoreSum += (dice[5] % 3) * 50;
    }
    else scoreSum += (dice[5]) * 50;        //If there are less than 3 dice with value 5, count individual fives (50pts
                                            //for each).

//If there are at least 3 dice with the value 2, dice[2] / 3 will give the number of groups of three twos (200pts for
//each group).
    if (dice[2] >= 3) scoreSum += (dice[2] / 3) * 200;

//If there are at least 3 dice with the value 3, dice[3] / 3 will give the number of groups of three threes (300pts for
//each group).
    if (dice[3] >= 3) scoreSum += (dice[3] / 3) * 300;

//If there are at least 3 dice with the value 4, dice[4] / 3 will give the number of groups of three fours (400pts for
//each group).
    if (dice[4] >= 3) scoreSum += (dice[4] / 3) * 400;

//If there are at least 3 dice with the value 6, dice[6] / 3 will give the number of groups of three sixes (600pts for
//each group).
    if (dice[6] >= 3) scoreSum += (dice[6] / 3) * 600;

//If there is a straight, there is no other possible ways to earn points. Set scoreSum to 1500 if there is a straight.
    for (int i = 1; i < 7; i++) {              //Testing for straight
        if ((dice[0] != 6 ) || (dice[i] != 1)) {
            straight = 0;
        }
    }
    if (straight) scoreSum = 1500;

    return scoreSum;
}
//selectDice Function: Removes dice values from dice array and moves them into keep array. Dice values are specified
//by the choice parameter. If an illegal digit is present in choice or one or more specified dice in choice do not
//exist in the dice array, the function returns zero, dice array does not change, and keep array is undefined.
int selectDice(DiceGroup dice, DiceGroup keep, int choice) {
    int retValue = 1;                          //initialize return value to 1
    int num = 0;
    int digits = 0;                            //number of digits in choice
    int base = 1;                              //base which will be used for integer division
    int isolated = 0;                          //isolated digit from choice
    int count1 = 0;                            //counters for die values (1-6)
    int count2 = 0;
    int count3 = 0;
    int count4 = 0;
    int count5 = 0;
    int count6 = 0;

    num = choice;
    while (num > 0) {                          //loop calculates number of digits in data
        digits++;
        num /= 10;
    }

    //Outer loop starts at rightmost digit and isolates each digit
    num = choice;
    for (int i = digits; i > 0; i--) {
        for (int exponent = (i - 1); exponent > 0; exponent--) {    //Inner loop calculates a power of 10 which
            base *= 10;                                             //will be used in integer division/modulo for
        }                                                           //isolating digits.
        isolated = (num / base) % 10;

        if ((isolated == 0) || (isolated > 6)) {                    //If isolated digit is invalid (zero or 7,8,9)
            retValue = 0;                                           //return value is zero; exit loop.
            break;
        }
        if ((dice[isolated]) == 0) {                                //If isolated digit (from choice) is not in dice array
            retValue = 0;                                           //return value is zero; exit loop.
            break;
        }
        base = 1;                                                    //reset base for next loop iteration

        if (isolated == 1) count1++;                                 //count instances of each dice value (1-6)
        if (isolated == 2) count2++;
        if (isolated == 3) count3++;
        if (isolated == 4) count4++;
        if (isolated == 5) count5++;
        if (isolated == 6) count6++;

    }
//If the there are more of a certain digit (1-6) in choice than in the dice array, return value is zero.
    if (dice[1] < count1) retValue = 0;
    if (dice[2] < count2) retValue = 0;
    if (dice[3] < count3) retValue = 0;
    if (dice[4] < count4) retValue = 0;
    if (dice[5] < count5) retValue = 0;
    if (dice[6] < count6) retValue = 0;


//Only change dice and keep if return value is one
if (retValue == 1) {
//Specified dice (from choice) are removed from dice array
    dice[0] -= digits;
    dice[1] -= count1;
    dice[2] -= count2;
    dice[3] -= count3;
    dice[4] -= count4;
    dice[5] -= count5;
    dice[6] -= count6;
//Specified dice (from choice) are present in keep array
    keep[0] = digits;
    keep[1] = count1;
    keep[2] = count2;
    keep[3] = count3;
    keep[4] = count4;
    keep[5] = count5;
    keep[6] = count6;

}
    return retValue;
}

//takeTurn Function: Implements one turn for one player and returns the score for that player's turn.
int takeTurn() {
    DiceGroup dice;                                     //dice array
    DiceGroup keep;                                     //keep array
    DiceGroup save;                                     //save array: used for saving values of dice array
    int choice = 0;                                     //choice variable for selectDice function
    int numDice = 6;                                    //farkle turn starts with rolling six dice
    char rollAgain;                                     //char variable for storing 'y' or 'n'
    int turnScore = 0;                                  //initialize score for the turn to zero
    int farkle = 0;                                     //initialize farkle truth value to false


    while (1) {
        rollDice(dice, numDice);                        //roll dice

        printf("Rolling %d dice...", dice[0]);

        printDice(dice);

        printf("\n");

        farkle = testFarkle(dice);                      //test for farkle; if player rolls farkle, turn is over and
                                                        //score is zero
        if (farkle) {
            printf("FARKLE -- your turn is over.\n");
            return 0;
        }

        for (int i = 0; i < 7; i++) {                   //copy contents of dice into save array
            save[i] = dice[i];
        }

        while (1) {
            printf("Which to keep? ");                  //prompt for dice to be kept; store in 'choice'
            scanf("%d", &choice);

            if (selectDice(dice, keep, choice) == 0) {  //If choice is not illegal, re-prompt for new choice value
                printf("No match, try again.\n");
                continue;
            }
            else {
                //If choice is legal, but keep dice score to zero, restore dice to original group, and re-prompt
                //for new choice value.
                if (scoreDice(keep) == 0) {
                    printf("Must keep scoring dice. Try again.\n");
                    for (int i = 0; i < 7; i++) {
                        dice[i] = save[i];
                    }
                    continue;
                }
                else break;
            }
        }


        printf("Keeping ");                                 //print dice to be kept for scoring and score for that roll
        printDice(keep);
        printf(", score = %d\n",scoreDice(keep));

        turnScore += scoreDice(keep);                       //add roll score to player's total score for turn

        printf("Score so far = %d\n", turnScore);           //print player's total score for turn so far

        numDice = dice[0];                                  //set numDice

        if (dice[0] == 0) {                                 //if there are no dice remaining after scoring dice removed,
            printf("HOT DICE!  Roll 6 dice (y/n)? ");       //Hot Dice occurs
        } else printf("%d dice left -- roll again (y/n)? ", dice[0]);

        scanf(" %c", &rollAgain);                           //Read character from player: 'y' = yes or 'n' = no

        if (rollAgain == 'y') {
            if (dice[0] == 0) numDice = 6;                  //If yes and there is Hot Dice, # of dice to roll is six
            else numDice = dice[0];                         //If yes and no Hot Dice, # of dice to roll is remaining dice
            continue;
        }
        else break;                                         //If no, turn is over; exit loop.
}
        return turnScore;                                   //Return player's total score for the turn
}


// RANDOM NUMBER FUNCTIONS
// Using 16-bit linear feedback shift register (LFSR) for pseudorandom number generator
unsigned int lfsr = 0;  // global variable -- state of random number generator

// lfsrNext -- function to advance an LFSR for pseudorandom number generation
// uses global variable (lfsr)
// code from: https://en.wikipedia.org/wiki/Linear-feedback_shift_register
unsigned int lfsrNext(void) {
    lfsr ^= lfsr >> 7;
    lfsr ^= lfsr << 9;
    lfsr ^= lfsr >> 13;
    return lfsr;
}

// set starting value for LFSR-based random number generator
void seedRandom(unsigned int seed) {
    if (seed) lfsr = seed;
}
// return a random integer between 0 and limit-1
unsigned int getRandom(unsigned int limit) {
    return lfsrNext() % limit;  //
}
