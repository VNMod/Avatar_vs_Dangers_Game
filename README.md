# Avatar_vs_Dangers_Game

### Date: April 2020

## Overview:

This is a 2-D game playable on TI's MSP432P401R microcontroller and viewable on the LCD screen attached on the BoosterPack MKII. The **objective** of the game is to avoid all the red dangers, score points by destroying them using temporary shields and collect yellow shield points to restock on shields. It is a small embedded software application that uses software timers, proper low-level graphics rendering techniques and is interfaced with an analog stick and buttons (on the boosterpack).

## Fulfilled Criteria:

- Developed a sizeable embedded software application that runs on the MSP432P401R and meets real-time specifications.
- Used low level graphics rendering techniques.
- Configured and Programmed analog stick and buttons for gameplay.
- Planned out and implemented a solid FSM model for the game.
- Used software timers for each state (of the game/FSM).
- Used event-driven programming methods, polling-based and interrupt-driven input/output.

## Gameplay:

- After the intro screen, the user is prompted with three options: play game, how to play and high scores.
- Obviously enough, 'how to play' displays the instructions for the player. He/she may choose to navigate back by pressing the joystick button.
- 'High Scores' keep track of the highest set of scores (with a capacity of 6). The scores are only kept track of as long as the MSP432 board isn't reset/rebooted.
- A white avatar is used by the player to survive the red dangers. You start out with 3 health points which are consequently deducted as you encounter dangers.
- The player can increase their score by a point by killing dangers using green shields that destroy any danger within its scope. In order to be able to use more shields, players must replenish their supply by collecting yellow shield points scattered around the game.
- If the avatar gets hit, it receives a temporary invulnerability time for seven seconds to prevent instant defeat upon getting hit by a danger.
- The game is over when all health points have been lost. The player can then choose to navigate back to the main menu by clicking the joystick button once again.

### **Note: For more info check out the PDF in this repository**
