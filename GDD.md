# Two Forts 1.99 - Game Design Document

## 1. Game Overview

Two Forts 1.99 is a fast-paced, top-down 2D shooter inspired by Tank 90. Two players face off in a confined arena.
The game focuses on reflexes, positioning and short but intense matches.<br><br>
The game supports two distinct modes:
- Arena Mode: Score-based, longer matches
- High Noon Mode: One-hit elimination duel

## 2. Gameplay Mechanics

The game is played from a top-down perspective. Each player controls a single character and competes against the other player in real time.
Core mechanics include:
- Movement: Players can move freely in four directions.
- Shooting: Players can fire projectiles in the direction they are facing. Projectiles travel in straight lines and are blocked by obstacles.
- Dash: Players can perform a quick dash in their current movement direction to evade attacks or reposition. Dash has a cooldown to prevent spamming.
- Power-Ups: Temporary and permanent power-ups spawn during gameplay and provide advantages.

## 3. Game Elements

### Players

- Two distinct players, each with identical abilities for fairness.
- Player 1 is MAN, Player 2 is MACHINE.

### Power-Ups

Power-ups spawn randomly in contested areas of the arena.
- Speed Boost: Temporarily increases movement speed.
- +1 Health: Permanently increases HP by 1 point. (Max HP is 3)
- Double Shot: Temporarily allows the player to fire two projectiles at once
- Barrage: Shoots bullets in a circular area after passing through it.
- Piercing Shot: Temporarily allows the player's projectile to go through obstacles.

### Maps

- Maps are enclosed spaces with rectangular objects.
- Obstacles block movement and projectiles.
- Wooden boxes are breakable obstacles while walls are not.

## 4. Progression and Goals

This game does not support long-term progression. Instead, progression is match-based.

### Arena Mode

- Players start with 3 HP.
- Each hit reduces HP by 1 point.
- Players respawn when they are hit.
- First player to lose 3 HP loses the game.
- Winner gets 1 point.

### High Noon Mode

- Players start with 1 HP.
- One hit results in elimination.
- No respawning.
- Winner gets 1 point.

## 5. Controls

### Player 1

- Move: W / A / S / D
- Shoot: SPACE
- Dash: L-SHIFT (5 second cooldown)

### Player 2

- Move: Arrow Keys
- Shoot: R-CTRL
- Dash: R-SHIFT (5 second cooldown)

## 6. Visual and Audio Style

### Visual Style

The visual design is intentionally simple and minimalistic.

- Gray tones were chosen for Arena Mode to resemble a place that is closer to MACHINE side's HQ.
- Brown tones were chosen for High Noon Mode to resemble a place that is closer to MAN side's HQ.
- Projectiles were colored blue and red for immidiate visual clarity.

### Audio Style

The audio design follows a retro-inspired approach, using primarily 16-bit style sound effects and music.
Sound effects were chosen to provide clear feedback for player actions. 

Audio feedback was provided for:

- Shooting
- Dashing
- Receiving Damage
- Projectile Collision
- Menu Navigation

Background music differs between game modes to reinforce their distinct moods.

## 7. Challenges and Solutions

### Challenge: Disappearing Menus

One major problem encountered during development was the main menu and sub-menus disappearing after navigation.
When player clicked "BACK" button, instead of returning to the Main Menu, the screen would turn black.
The same issue occurred when exiting other sub-menus.<br><br>
The main cause of this problem was the use of multiple enum-based game state definitions spread across multiple source files.
To solve the problem, a single file name GameState.h was created, storing every state of the game.

### Challenge: Characters Spawning Inside Obstacles

At the beginning of development, players would sometimes spawn inside walls or breakable objects.
This made certain rounds unfair or completely unplayable, as a player could become stuck and become vulnerable.<br><br>
The issue occurred because spawn positions were chosen randomly without validating them against the arena’s collision geometry.
As a solution, A safe spawn algorithm was implemented. This system repeatedly generates random spawn positions and checks them 
against the map geometry using raylib’s collision detection functions. A position is only accepted if it does not intersect 
with any obstacles or boundaries, ensuring fair and playable spawns every round.

### Challenge: Is It Really High Noon If There Are Multiple Maps ?

High Noon mode was designed as a single, focused duel taking place on a fixed map.
However, the existing game flow forced players to select an arena before starting any match,
which conflicted with the intended gameplay experience of High Noon mode.<br><br>
To solve the problem, the game state transition logic was modified so that High Noon mode bypasses the map selection screen entirely.
When this mode is selected, the game automatically loads a predefined desert arena, preserving the intended one-shot duel experience.
