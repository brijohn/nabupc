/*
 * Breakout
 * Brian Johnson (c)2023
 *
 * Simple version of the breakout game.
 *
 * This game requires the use of a paddle controller to play.
 *
 * This is based on the Brick Battle code released by DJ Sures.
 *
 *
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define BIN_TYPE BIN_HOMEBREW

#include "NABU-LIB.h"
#include "patterns.c"

unsigned char SPRITE_DATA[]={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x7F,0xFF,0xFF,0x7F,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xFE,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x38,0x7C,0xFE,0xFE,0xFE,0x7C,0x38,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

#define SPRITE_NAME_BLANK  0
#define SPRITE_NAME_PADDLE 1
#define SPRITE_NAME_BALL   2

#define SCREEN_MAX_X 255
#define SCREEN_MIN_X 1

#define SCREEN_MAX_Y 190
#define SCREEN_MIN_Y 8


uint8_t tb[64] = { 0x00 };

uint16_t _loopCntr = 0;

typedef struct {
	uint8_t  x;
	uint8_t  y;
	uint16_t score;
	uint8_t  balls;
	bool     ballInPlay;
	uint8_t  spriteId;
	bool     gameOver;
	bool     vdpReady;
} _player;

typedef struct {
	uint8_t  x;
	uint8_t  y;
	bool     xDir;
	bool     yDir;
	uint8_t  xSpeed;
	uint8_t  ySpeed;
	uint8_t  spriteId;
	bool     vdpReady;
} _ball;

_player player;
_ball ball;

void updateStatus()
{

	sprintf(tb, "%05u", player.score);

	vdp_setCursor2(2, 0);
	vdp_print(tb);

	sprintf(tb, "BALLS: %02u", player.balls);
	vdp_setCursor2(20, 0);
	vdp_print(tb);
}

void initBall()
{
	ball.x = player.x + 4;
	ball.y = player.y - 8;
	ball.xSpeed = 2;
	ball.ySpeed = 2;
	ball.xDir = true;
	ball.yDir = false;
	ball.vdpReady = true;
	ball.spriteId = vdp_spriteInit(4, SPRITE_NAME_BALL, ball.x, ball.y, VDP_WHITE);
}

void initPlayer()
{

	uint8_t x = (SCREEN_MAX_X / 2) - 8;

	player.x        = x;
	player.y        = (uint8_t)(SCREEN_MAX_Y - 14);
	player.score    = 0;
	player.balls    = 2;
	player.ballInPlay = false;
	player.gameOver = false;
	player.vdpReady = true;
	player.spriteId = vdp_spriteInit(0, SPRITE_NAME_PADDLE, player.x, player.y, VDP_WHITE);

	updateStatus();
}

void checkBrickCollision()
{
	uint8_t xCol = (ball.x + 4) / 8;
	uint8_t yCol = (ball.y + 4) / 8;
	if (vdp_getCharAtLocationBuf(xCol, yCol) >= 10) {
		vdp_putPattern(xCol, yCol, 0x00);
		switch(xCol % 3) {
		case 0:
			vdp_putPattern(xCol - 1, yCol, 0x00);
			vdp_putPattern(xCol - 2, yCol, 0x00);
			break;
		case 1:
			vdp_putPattern(xCol + 1, yCol, 0x00);
			vdp_putPattern(xCol + 2, yCol, 0x00);
			break;
		case 2:
			vdp_putPattern(xCol + 1, yCol, 0x00);
			vdp_putPattern(xCol - 1, yCol, 0x00);
			break;
		}
		ball.yDir = !ball.yDir;
		switch (yCol) {
		case 9:
		case 8:
			player.score += 1;
			break;
		case 7:
		case 6:
			player.score += 3;
			break;
		case 5:
		case 4:
			player.score += 5;
			break;
		case 3:
		case 2:
			player.score += 7;
			break;
		}
		updateStatus();
		playNoteDelay(0, 50, 10);
	}
}

void doPlayer()
{
	uint8_t x = getPaddleValue(0);
	if (x < SCREEN_MIN_X + 1) {
		x = SCREEN_MIN_X + 1;
	}

	if (x > SCREEN_MAX_X - 17) {
		x = (uint8_t)(SCREEN_MAX_X - 17);
	}

	player.vdpReady = true;
	player.x = x;

	if (!player.ballInPlay) {
		if (getJoyStatus(0) & Joy_Left) {
			player.ballInPlay = true;
		}
	}
}

void doBall()
{
	bool needsRedraw = false;

	if (player.ballInPlay) {
		if (!(_loopCntr % ball.xSpeed)) {
			if (ball.xDir) {
				ball.x++;
				if (ball.x >= SCREEN_MAX_X - 8) {
					playNoteDelay(2, 0, 10);
					ball.xDir = false;
				}
			} else {
				ball.x--;
				if (ball.x <= SCREEN_MIN_X) {
					playNoteDelay(2, 0, 10);
					ball.xDir = true;
				}
			}
			needsRedraw = true;
		}
		if (!(_loopCntr % ball.ySpeed)) {
			if (ball.yDir) {
				ball.y++;
				if (ball.y >= SCREEN_MAX_Y - 6) {
					playNoteDelay(2, 0, 10);
					ball.yDir = false;
					player.ballInPlay = false;
					if (player.balls > 0) {
						player.balls--;
						updateStatus();
					} else {
						player.gameOver = true;
					}
				}
				checkBrickCollision();
			} else {
				ball.y--;
				if (ball.y <= SCREEN_MIN_Y) {
					playNoteDelay(2, 0, 10);
					ball.yDir = true;
				}
				checkBrickCollision();
			}
			needsRedraw = true;
		}
		if (needsRedraw) {
			ball.vdpReady = true;
		}
	} else {
		ball.x = player.x + 4;
		ball.y = player.y - 8;
		ball.vdpReady = true;
	}
}

void doCollision()
{
	// width and height of sprite
	uint8_t xd = player.x + 16;
	uint8_t yd = player.y + 4;

	// center of paddle
	uint8_t xc = player.x + 8;
	uint8_t yc = player.y + 2;

	// abs value of ball center from center of paddle
	uint8_t rel_pos = abs((ball.x + 4) - xc);

	if ((player.x < ball.x + 16) && (xd > ball.x) &&
	    (player.y < ball.y + 8) && (yd > ball.y)) {

		playNoteDelay(1, 0, 10);

		bool xDir = ball.xDir;

		if (xc < ball.x + 4) {
			ball.ySpeed = xDir ? 2 : 1;
			ball.xDir = true;
		} else {
			ball.ySpeed = !xDir ? 2 : 1;
			ball.xDir = false;
		}

		ball.yDir = false;

		if (rel_pos <= 3) {
			ball.xSpeed = 2;
		} else {
			ball.xSpeed = 1;
		}
	}
}

void initBricks()
{
	uint8_t id = 10;
	for (uint8_t y = 2; y < 10; y++) {
		uint8_t p = 0;
		for (uint8_t x = 1; x < 31; x++) {
			vdp_putPattern(x, y, id + (p % 3));
			p++;
		}
		id += (y % 2 == 1) ? 3 : 0;
	}
}

void mainMenu()
{
	vdp_setCursor2(11, 5);
	vdp_print("BREAKOUT");

	vdp_setCursor2(5, 10);

	vdp_print("PRESS ENTER TO START");
}

void clearScreen() {

	vdp_fillScreen(0x00);

	for (uint8_t x = 0; x < 32; x++) {
		vdp_putPattern(x, 1, 0x03);
	}

	for (uint8_t y = 1; y < 23; y++) {
		vdp_putPattern(0, y, 0x02);
		vdp_putPattern(31, y, 0x08);
	}

	vdp_putPattern(0, 1, 0x01);
	vdp_putPattern(31, 1, 0x05);
}

void doGameLoop()
{
	clearScreen();

	initPlayer();

	initBall();

	initBricks();

	while (true)
	{
		if (vdpStatusRegVal & VDP_FLAG_COLLISION)
			doCollision();

		doPlayer();
		doBall();

		vdp_waitVDPReadyInt();

		if (player.vdpReady) {
			vdp_setSpritePosition(player.spriteId, player.x, player.y);
			player.vdpReady = false;
		}

		if (ball.vdpReady) {
			vdp_setSpritePosition(ball.spriteId, ball.x, ball.y);
			ball.vdpReady = false;
		}
		_loopCntr++;

		if (player.gameOver) {
			vdp_setCursor2(11, 11);
			vdp_print("GAMEOVER");
			while (true) {
				if (getJoyStatus(0) & Joy_Left) {
					break;
				}
			}
			break;
		}
	}
}

void main(void) {
	initNABULib();
	vdp_clearVRAM();
	vdp_initG2Mode(0, true, false, false, false);

	vdp_loadPatternTable(PATTERN, sizeof(PATTERN));
	vdp_loadColorTable(COLOR, sizeof(COLOR));
	vdp_loadSpritePatternNameTable(3, SPRITE_DATA);

	for (uint8_t i = 33; i < 91; i++)
		vdp_colorizePattern(i, VDP_WHITE, VDP_TRANSPARENT);

	vdp_enableVDPReadyInt();

	mainMenu();

	while(true) {
		vdp_waitVDPReadyInt();
		if (isKeyPressed() && getChar() == 0x0D) {
			doGameLoop();
			vdp_fillScreen(0x00);
			vdp_disableSprite(0);
			vdp_disableSprite(4);
			mainMenu();
		}
	}
}
