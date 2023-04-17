/*****************************************************************************************
 ** Program Filename: consts.h
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Contains all of the constants for the entire program. It's everything 
 **				 from sizes to color stuff to actual text printed on the screen.
 ** Input: none.
 ** Output: none.
 ****************************************************************************************/
#ifndef __consts_h__
#define __consts_h__

// NOTE: strings are always null terminated

#define MIN_COLS 50
#define MIN_LINES 25


/************************************  PANEL CONSTS  ************************************/

// redefines all terminal colors, but the color range is from 0 to 1000...
// #0099ff
#define PANEL_BLUE_R 0
#define PANEL_BLUE_G 600
#define PANEL_BLUE_B 1000

// #eb391b
#define PANEL_RED_R 922
#define PANEL_RED_G 224
#define PANEL_RED_B 106

// #2eb82e
#define PANEL_GREEN_R 180
#define PANEL_GREEN_G 722
#define PANEL_GREEN_B 180

// #808080, also no sys default for "grey" so this one redefines COLOR_CYAN
#define PANEL_GREY_R 500
#define PANEL_GREY_G 500
#define PANEL_GREY_B 500


/*********************************  START PANEL CONSTS  *********************************/

#define START_CPAIR_TITLE 1
#define START_CPAIR_DISABLED 2

#define START_MIN_MAZE_SIZE 5
#define START_MAX_MAZE_SIZE 999

#define START_BUTTON_SQUARE 0
#define START_BUTTON_WIDTH 1
#define START_BUTTON_HEIGHT 2
#define START_BUTTON_START 3
#define START_BUTTON_OPTIONS 4
#define START_BUTTON_QUIT 5

#define START_TEXT_TITLE "Escape from CS 162"
#define START_TEXT_SQUARE "use a square maze? "
#define START_TEXT_WIDTH "maze width: "
#define START_TEXT_HEIGHT "maze height: "
#define START_TEXT_START "START"
#define START_TEXT_OPTIONS "SETTINGS"
#define START_TEXT_QUIT "QUIT"

/**********************************  GAME PANEL CONSTS  *********************************/
#define GAME_HUD_HEIGHT 5
#define GAME_HUD_WIDTH_1 25
#define GAME_HUD_WIDTH_2 30

#define GAME_HUD_TEXT_QUIT "Q to quit"
#define GAME_HUD_TEXT_SKILLS "Num skills: "
#define GAME_HUD_TEXT_TA "TAs: "
#define GAME_HUD_TEXT_MOVE "WASD or arrows to move"
#define GAME_HUD_TEXT_CAMERA "IJKL to move camera"
#define GAME_HUD_TEXT_DEMO "P to demo a skill"
#define GAME_HUD_TEXT_AI "SPACE or ENTER to advance"

#define GAME_CPAIR_PLAYER 1
#define GAME_CPAIR_TA_INSTRUCTOR 2
#define GAME_CPAIR_TA_DISABLED 3
#define GAME_CPAIR_SKILL 4
#define GAME_CPAIR_WALL_FOG 5

#define GAME_SQUARE_SIZE 3		// multiple of 3 for best results

/**********************************  GAME ALERT CONSTS  *********************************/

#define ALERT_QUIT 0
#define ALERT_WON 1
#define ALERT_LOST 2
#define ALERT_TUTORIAL_ASK 3
#define ALERT_TUTORIAL_1 4
#define ALERT_TUTORIAL_2 5
#define ALERT_TUTORIAL_3 6
#define ALERT_TUTORIAL_4 7
#define ALERT_TUTORIAL_5 8
#define ALERT_TUTORIAL_6 9
#define ALERT_TUTORIAL_FOG 10
#define ALERT_TUTORIAL_AI 11

#define ALERT_ROWS 16			// 1 + a multiple of 3 for best results
#define ALERT_COLS 40

// left/right margin (between text and border, only applies to text)
#define ALERT_MARGIN 5
// vertical margin (between text, buttons, and border but border fills 1)
#define ALERT_ELEMENT_MARGIN 2
// min space between buttons (and border)
#define ALERT_MIN_BUTTON_SPACE 1

#define ALERT_QUIT_TEXT "Are you sure you want to quit?"
#define ALERT_WON_TEXT "Congrats! You won!"
#define ALERT_LOST_TEXT "Oh nooo! Want to try again?"
#define ALERT_TUTORIAL_ASK_TEXT "Which tutorial do you want?"

#define ALERT_TUTORIAL_1_TEXT_1 "You're trying to pass CS 162. Can't"
#define ALERT_TUTORIAL_1_TEXT_2 "imagine that. Anyway, you find"
#define ALERT_TUTORIAL_1_TEXT_3 "yourself stuck in this maze with your"
#define ALERT_TUTORIAL_1_TEXT_4 "instructor and some TAs."

#define ALERT_TUTORIAL_2_TEXT_1 "You are represented by the green Y,"
#define ALERT_TUTORIAL_2_TEXT_2 "which stands for \"you.\" TAs are"
#define ALERT_TUTORIAL_2_TEXT_3 "the red Ts, and your instructor is"
#define ALERT_TUTORIAL_2_TEXT_4 "the red I. You collect programming"
#define ALERT_TUTORIAL_2_TEXT_5 "skills, noted by a blue $."

#define ALERT_TUTORIAL_3_TEXT_1 "When you encounter a TA, you get"
#define ALERT_TUTORIAL_3_TEXT_2 "super frightened and freeze up. You"
#define ALERT_TUTORIAL_3_TEXT_3 "die of a sudden heart attack. The"
#define ALERT_TUTORIAL_3_TEXT_4 "only way to survive an encounter with"
#define ALERT_TUTORIAL_3_TEXT_5 "a TA is to demonstrate a programming"
#define ALERT_TUTORIAL_3_TEXT_6 "skill before you meet them and appease"
#define ALERT_TUTORIAL_3_TEXT_7 "all of them for a bit."

#define ALERT_TUTORIAL_4_TEXT_1 "When you encounter your instructor,"
#define ALERT_TUTORIAL_4_TEXT_2 "you get your final exam, right then"
#define ALERT_TUTORIAL_4_TEXT_3 "and there. Where's everyone else?"
#define ALERT_TUTORIAL_4_TEXT_4 "Doesn't matter. If you've learned at"
#define ALERT_TUTORIAL_4_TEXT_5 "least 3 skills (only?), you'll pass."
#define ALERT_TUTORIAL_4_TEXT_6 "If not, you fail the class. No"
#define ALERT_TUTORIAL_4_TEXT_7 "buts. Talk about harsh..."

#define ALERT_TUTORIAL_5_TEXT_1 "You move with WASD (or arrow keys,"
#define ALERT_TUTORIAL_5_TEXT_2 "whichever you prefer). To demonstrate"
#define ALERT_TUTORIAL_5_TEXT_3 "a programming skill to all the TAs,"
#define ALERT_TUTORIAL_5_TEXT_4 "click P. If, for some reason you don't"
#define ALERT_TUTORIAL_5_TEXT_5 "want to move, you can hit SPACE or ENTER."

#define ALERT_TUTORIAL_6_TEXT_1 "To move the camera, you use IJKL."
#define ALERT_TUTORIAL_6_TEXT_2 "All of these controls are also on"
#define ALERT_TUTORIAL_6_TEXT_3 "the HUD up top (along with TA status"
#define ALERT_TUTORIAL_6_TEXT_4 "and number of programming skills)."

#define ALERT_TUTORIAL_FOG_TEXT_1 "FOG OF WAR"
#define ALERT_TUTORIAL_FOG_TEXT_2 ""
#define ALERT_TUTORIAL_FOG_TEXT_3 "The room is dark. You have your phone"
#define ALERT_TUTORIAL_FOG_TEXT_4 "with you, but it doesn't illuminate"
#define ALERT_TUTORIAL_FOG_TEXT_5 "much. You can only see so far in any"
#define ALERT_TUTORIAL_FOG_TEXT_6 "direction."

#define ALERT_TUTORIAL_AI_TEXT_1 "Sometimes you just don't want to do"
#define ALERT_TUTORIAL_AI_TEXT_2 "your work. Such is with your CS 162"
#define ALERT_TUTORIAL_AI_TEXT_3 "class. I mean, it's a class about"
#define ALERT_TUTORIAL_AI_TEXT_4 "coding, after all, so you made a little"
#define ALERT_TUTORIAL_AI_TEXT_5 "program to tell you where to go. How"
#define ALERT_TUTORIAL_AI_TEXT_6 "effective is it? Only time will tell."

#define ALERT_BUTTON_QUIT "QUIT"
#define ALERT_BUTTON_AGAIN "PLAY AGAIN"
#define ALERT_BUTTON_YES "YES"
#define ALERT_BUTTON_NO "NO"

// only for tutorials 
#define ALERT_BUTTON_OK "OKAY"
#define ALERT_BUTTON_NONE "NONE"
#define ALERT_BUTTON_NORMAL_TUT "...YES"
#define ALERT_BUTTON_FOG_TUT "FOR FOG"
#define ALERT_BUTTON_AI_TUT "FOR AI"
#define ALERT_BUTTON_ANOTHER "ANOTHER!"
#define ALERT_BUTTON_PLAY "LET'S GO"


/************************************  MAZE CONSTS  *************************************/

// the number of stuff is a linear expression where these are the const parts
// these should be the actual values when we have min maze size (5x5)
// also affected by difficulty multipliers
#define MAZE_MIN_WALLS 4
#define MAZE_MIN_TAS 3
#define MAZE_MIN_SKILLS 6

// number of stuff is a linear expression where these are the scaled parts
#define MAZE_SCALE 25
#define MAZE_WALL_PER_SCALE 3
#define MAZE_TA_PER_SCALE 2
#define MAZE_SKILLS_PER_SCALE 2

// difficulty multipliers, sign based on difficulty (easy is -, med is 0, hard is +)
// it's 1 + diff_mult*difficulty (so val of 0.5 makes it a multiplier of 1.5)
#define MAZE_DIFF_MULT_TA 0.25
#define MAZE_DIFF_MULT_WALL 0.5
#define MAZE_DIFF_MULT_SKILLS -0.5

#define MAZE_FOG_SIZE 3

// moves
#define MAZE_MOVE_NONE 0
#define MAZE_MOVE_UP 1
#define MAZE_MOVE_DOWN 2
#define MAZE_MOVE_LEFT 3
#define MAZE_MOVE_RIGHT 4

#define TA_TEXT_APPEASED_1 "Appeased ("
#define TA_TEXT_APPEASED_2 " turns)"
#define TA_TEXT_NOT_APPEASED "On the hunt"

#define DISPLAY_CHAR_TA 'T'
#define DISPLAY_CHAR_STUDENT 'Y'
#define DISPLAY_CHAR_INSTRUCTOR 'I'
#define DISPLAY_CHAR_SKILL '$'
#define DISPLAY_CHAR_WALL '#'
#define DISPLAY_CHAR_EMPTY ' '

/***********************************  OPTION CONSTS  ************************************/

#define OPTION_FILE_NAME "options.txt"

// also used for the start panel isSquare
#define OPTION_CHECK_OFF ' '
#define OPTION_CHECK_ON 'X'

#define OPTION_LEFT_MARGIN 10
#define OPTION_DIFF_DESC "Difficulty: "
#define OPTION_FOG_DESC "Fog of war: "
#define OPTION_AI_DESC "Let the AI out to play: "		// 24 chars
#define OPTION_BACK_DESC "Back"

#define OPTION_BUTTON_BACK 0
#define OPTION_BUTTON_DIFF 1
#define OPTION_BUTTON_FOG 2
#define OPTION_BUTTON_AI 3

#define OPTION_DIFF_EASY -1
#define OPTION_DIFF_MED 0
#define OPTION_DIFF_HARD 1

#define OPTION_DIFF_EASY_DESC " Easy "
#define OPTION_DIFF_MED_DESC " Medium "
#define OPTION_DIFF_HARD_DESC " Hard "

#define OPTION_DIFF_DEFAULT OPTION_DIFF_MED
#define OPTION_FOG_DEFAULT false
#define OPTION_AI_DEFAULT false

#endif