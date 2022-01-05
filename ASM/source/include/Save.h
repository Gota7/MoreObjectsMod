#ifndef SM64DS_SAVE_INCLUDED
#define SM64DS_SAVE_INCLUDED

#include "SM64DS_Common.h"

constexpr int NUM_LEVELS = 52;
constexpr int NUM_MAIN_LEVELS = 15;
constexpr int NUM_SECRET_LEVELS = 15;
constexpr int NUM_MINIGAMES = 36;


struct SaveData
{
	uint32_t magic8000;			//8000 in ASCII
	uint8_t keysObtained;		/*	0x01: File saved //set when file is saved
									0x02: Basement key obtained
									0x04: Upper hall key obtained		//Removes sub from DDD
									0x08: Mario key obtained
									0x10: Luigi key obtained
									0x20: Wario key obtained
									0x40: Main hall key obtained
									0x80: White key obtained	//Triggers white room star */
	uint8_t keysUsed;			/*	0x01: Basement key used
									0x02: Upper hall key used	//Removes sub from DDD
									0x04: Mario key used
									0x08: Luigi key used
									0x10: Wario key used
									0x20: Main hall key used
									0x40: White key used	//Disables white room star
									0x80: WF star door open	*/
	uint8_t starDoorState;		/*	0x01: Character star door open
									0x02: JRB star door open
									0x04: CCM star door open
									0x08: Mario star door open
									0x10: Bowser 1 door open
									0x20: Bowser 2 door open
									0x40: Clock door open
									0x80: ??? //Unused */
	uint8_t miscStates1;		/*	0x01: Mario lost his cap
									0x02: Luigi lost his cap
									0x04: Wario lost his cap
									0x08: ??? //Unused
									0x10: ??? //Unused
									0x20: ??? //Unused
									0x40: Minigames activated //talked to minigame toad
									0x80: Red switch activated //if not set, the shine in main hall is rendered */
	uint8_t characterState;		/*	0x01: Mario intro
									0x02: Luigi intro
									0x04: Wario intro
									0x08: Always set, marks started save
									0x10: ???	//Unused
									0x20: ???	//Unused
									0x40: ???	//Unused
									0x80: Character control		//Getting control of character */
	uint8_t textState;			/*	0x01: ??? //Unused
									0x02: Bowser intro
									0x04: Feather intro
									0x08: Balloon intro
									0x10: Invisibility intro
									0x20: Metal cap intro
									0x40: Fire breath intro
									0x80: Bomb-omb intro */
	uint8_t miscStates2;		/*	0x01: Door text activated, Lakitu trigger
									0x02: Lakitu done
									0x04: DDD image moved  //set every frame if DDD star 1 is collected & player is in basement
									0x08: Water drained
									0x10: Glowing rabbit 1
									0x20: Glowing rabbit 2
									0x40: Glowing rabbit 3
									0x80: Glowing rabbit 4 */
	uint8_t miscStates3;		/*	0x01: Glowing rabbit 5
									0x02: Glowing rabbit 6
									0x04: Glowing rabbit 7
									0x08: Glowing rabbit 8
									0x10: Mario cap intro
									0x20: ??? //Unused
									0x40: ??? //Unused
									0x80: ??? //Unused */
	uint8_t minigameRabbits1;	/*	0x01: Left bridge (M)
									0x02: Backyard (L)
									0x04: Castle right (W)
									0x08: Castle 1 (Y)
									0x10: Aquarium room (M)
									0x20: Water drainage (L)
									0x40: TTM room (W)
									0x80: Castle 2 (Y)	*/
	uint8_t minigameRabbits2;	/*	0x01: Mirror room (M)
									0x02: Mario room (L)
									0x04: Basement (W)
									0x08: Castle 3 (Y)
									0x10: Basement 1 (M)
									0x20: BOB room (L)
									0x40: Backyard (W)
									0x80: Castle right (Y)	*/
	uint8_t minigameRabbits3;	/*	0x01: Basement 2 (M)
									0x02: Hedge maze (L)
									0x04: THI room (W)
									0x08: Hedge maze (Y)
									0x10: Moat (M)
									0x20: 2nd floor (L)
									0x40: 3rd floor 1 (W)
									0x80: Basement (Y)	*/
	uint8_t minigameRabbits4;	/*	0x01: Upper hall (M)
									0x02: Castle roof (L)
									0x04: 3rd floor 2 (W)
									0x08: Moat (Y)
									0x10: ??? //Unused
									0x20: ??? //Unused
									0x40: ??? //Unused
									0x80: ??? //Unused	*/
	uint8_t cannonUnlocked1;	/*	0x01: BOB
									0x02: WF
									0x04: JRB
									0x08: CCM
									0x10: ??? //Unused
									0x20: ??? //Unused
									0x40: ??? //Unused
									0x80: SSL	*/
	uint8_t cannonUnlocked2;	/*	0x01: ??? //Unused
									0x02: SL
									0x04: WDW
									0x08: TTM
									0x10: THI
									0x20: ??? //Unused
									0x40: RR
									0x80: ??? //Unused	*/
	uint8_t cannonUnlocked3;	/*	//Unused	*/
	uint8_t cannonUnlocked4;	/*	0x01: ??? //Unused
									0x02: ??? //Unused
									0x04: OTR
									0x08: ??? //Unused
									0x10: ??? //Unused
									0x20: ??? //Unused
									0x40: ??? //Unused
									0x80: ??? //Unused	*/
	uint8_t stars[NUM_MAIN_LEVELS + NUM_SECRET_LEVELS]; // Main levels: 0xFE (7 stars) & 0x01 (100 coins)
								 /* Secret levels:
								 	15: Bowser 1 (0x02=Red, 0x04=Time)
									16: Bowser 2 (0x02=Red, 0x04=Time)
									17: Bowser 3 (0x02=Red, 0x04=Time)
									18: Mario painting (0x02=Red, 0x04=Time)
									19: Luigi painting (0x02=Red, 0x04=Std)
									20: Wario painting (0x02=Red, 0x04=Wario)
									21: Slide stars (0x02=Std, 0x04=20s)
									22: Water temple (0x02=Red)
									23: Switch palace (0x02=Red)
									24: Moat secret (0x02=Red, 0x04=Time)
									25: Behind the waterfall (0x02=Red, 0x04=Std)
									26: Rainbow secret (0x02=Red, 0x04=Wario)
									27: Beach painting (0x02=SS)
									28: Battle fort (0x02=SS)
									29: Castle secrets (0x02=Red, 0x04=Toad[HMC], 0x08=Toad[2ndF], 0x10=Toad[3rdF], 0x20=White[2ndF], 0x40=White[CR]) */

	uint8_t coinRecords[NUM_MAIN_LEVELS];		//Coin counts in each main level
	uint8_t currentCharacter;				/* (Mario=0, Luigi=1, Wario=2, Yoshi=3)	*/
	uint8_t unused_0x42[2];					//Always 0
	uint32_t magic5000;						//5000 in ASCII
	uint8_t minigamesUnlocked[4];			/*	Minigame bits: 0x1=Mario, 0x2=Luigi, 0x4=Wario, 0x8=Yoshi; minigame location: P=Page, R=Row, C=Column
												0: Minigames unlocked 1
												0x0n: P1R1C3
												0xn0: P1R2C1
												1: Minigames unlocked 2
												0x0n: P1R2C2
												0xn0: P1R2C3
												2: Minigames unlocked 3
												0x0n: P2R1C1
												0xn0: P2R1C2
												3: Minigames unlocked 4
												0x0n: P2R1C3
												0x10: ??? //Unused
												0x20: ??? //Unused
												0x40: ??? //Unused
												0x80: Always set, marks started minigame save	*/
	uint8_t minigamesPlayed[5];				/*	Minigames played 1-4 (exact same IDs like minigamesUnlocked, upper bits (4-7) of [3] are unused)	
												Minigames played 5:
													0x0n: P1R1C1
													0xn0: P1R1C2	*/
	uint8_t unused_0x51[3];						//Unused
	uint32_t minigameRecords[NUM_MINIGAMES][5];	/*	Minigame records 1-36; each minigame stores 5 scores */
	uint8_t unused_0x324[4];						//Unused
}
extern SAVE_DATA;


/* 	All unused fields can be safely used for custom purposes. A full function trackdown where every write is noted would be too tedious; 
	Rather, writes to SaveData have been trapped by triggering in-game events. Flags have been compared with real-world savefiles to ensure nothing is missing. 
	However, this may not imply they are really unused. Some may store states that trigger minor game events (like the cap lost flags). 
	In case you discovered such a state flag, please post it on the repo.	*/


#endif // SM64DS_SAVE_INCLUDED