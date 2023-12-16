/*
 * Pong
 * Brian Johnson (c)2023
 *
 * Paddle based version of pong for the NABU
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
0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,
0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,
0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,
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

#define MAX_PLAYERS 2

uint8_t tb[64] = { 0x00 };
uint16_t _loopCntr = 0;

typedef struct {
	uint8_t  x;
	uint8_t  y;
	uint16_t score;
	uint8_t  spriteId;
	bool     vdpReady;
} _player;

typedef struct {
	uint8_t  x;
	uint8_t  y;
	uint8_t  speed;
	bool     xDir;
	bool     yDir;
	uint8_t  spriteId;
	bool     vdpReady;
} _ball;


_player players[2];
_ball   ball;

void updateStatus()
{

	sprintf(tb, "PLAYER 1: %02u", players[0].score);

	vdp_setCursor2(1, 0);
	vdp_print(tb);

	sprintf(tb, "PLAYER 2: %02u", players[1].score);
	vdp_setCursor2(19, 0);
	vdp_print(tb);
}


void initPlayer()
{

	uint8_t y = (SCREEN_MAX_Y/2) - 8;

	players[0].x     = SCREEN_MIN_X;
	players[0].y     = y;
	players[0].score = 0;
	players[0].vdpReady = true;
	players[0].spriteId = vdp_spriteInit(0, SPRITE_NAME_PADDLE, players[0].x, players[0].y, VDP_WHITE);

	players[1].x     = (uint8_t)(SCREEN_MAX_X - 16);
	players[1].y     = y;
	players[1].score = 0;
	players[1].vdpReady = true;
	players[1].spriteId = vdp_spriteInit(1, SPRITE_NAME_PADDLE, players[1].x, players[1].y, VDP_WHITE);

	updateStatus();
}

void initBall()
{
	ball.x = 124;
	ball.y = 88;
	ball.speed = 1;
	ball.xDir = true;
	ball.yDir = false;
	ball.vdpReady = true;
	ball.spriteId = vdp_spriteInit(4, SPRITE_NAME_BALL, ball.x, ball.y, VDP_WHITE);
}

void addScore(uint8_t player)
{
	players[player].score += 1;
	updateStatus();
	ball.x = 124;
	ball.y = 88;
	ball.speed = 1;
}

void updatePlayers()
{
	for (int i = 0; i < MAX_PLAYERS; ++i) {

		uint8_t y = getPaddleValue(i);

		if (y < SCREEN_MIN_Y + 1) {
			y = SCREEN_MIN_Y + 1;
		}

		if (y > SCREEN_MAX_Y - 17) {
			y = (uint8_t)(SCREEN_MAX_Y - 17);
		}

		players[i].vdpReady = true;
		players[i].y = y;

	}
}

void updateBall()
{
	if (ball.xDir) {
		ball.x+=ball.speed;
		if (ball.x >= SCREEN_MAX_X - 8) {
			playNoteDelay(2, 0, 10);
			ball.xDir = false;
			ball.x = (uint8_t)(SCREEN_MAX_X - 8);
			addScore(0);
		}
	} else {
		ball.x-=ball.speed;
		if (ball.x <= SCREEN_MIN_X) {
			playNoteDelay(2, 0, 10);
			ball.xDir = true;
			ball.x = SCREEN_MIN_X;
			addScore(1);
		}
	}
	if (ball.yDir) {
		ball.y+=ball.speed;
		if (ball.y >= SCREEN_MAX_Y - 8) {
			playNoteDelay(2, 0, 10);
			ball.yDir = false;
			ball.y = (uint8_t)(SCREEN_MAX_Y - 8);
		}
	} else {
		ball.y-=ball.speed;
		if (ball.y <= SCREEN_MIN_Y) {
			playNoteDelay(2, 0, 10);
			ball.yDir = true;
			ball.y = SCREEN_MIN_Y;
		}
	}
	ball.vdpReady = true;
}

void checkCollision()
{

	//width and height of sprite
	for (int i = 0; i < MAX_PLAYERS; i++) {
	uint8_t xd = players[i].x + 10;
	uint8_t yd = players[i].y + 16;

	// center of paddle
	uint8_t xc = players[i].x + 2;
	uint8_t yc = players[i].y + 8;

	// abs value of ball center from center of paddle
	uint8_t rel_pos = abs((ball.y + 4) - yc);

	if ((players[i].x < ball.x + 8) && (xd > ball.x) &&
	    (players[i].y < ball.y + 16) && (yd > ball.y)) {

		playNoteDelay(1, 0, 10);

		bool xDir = ball.xDir;

		if (xc < ball.y + 4) {
//			ball.ySpeed = xDir ? 2 : 1;
			ball.xDir = true;
		} else {
//			ball.ySpeed = !xDir ? 2 : 1;
			ball.xDir = false;
		}

		ball.yDir = false;

		if (rel_pos <= 3) {
			ball.speed = 1;
		} else {
			ball.speed = 2;
		}
	}
	}
}

void clearScreen() {

	vdp_fillScreen(0x00);

	for (uint8_t x = 0; x < 32; x++) {
		vdp_putPattern(x, 1, 0x04);
		vdp_putPattern(x, 23, 0x05);
	}

	for (uint8_t y = 1; y < 24; y++) {
		vdp_putPattern(15, y, 0x01);
	}
	vdp_putPattern(15, 1, 0x02);
	vdp_putPattern(15, 23, 0x03);
}

void mainMenu()
{
	vdp_setCursor2(13, 5);
	vdp_print("PONG");

	vdp_setCursor2(5, 10);

	vdp_print("PRESS ENTER TO START");
}

bool checkGameover()
{
	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (players[i].score >= 11) {
			sprintf(tb, "PLAYER %d WINS!", i+1);
			vdp_setCursor2(9, 11);
			vdp_print(tb);
			return true;
		}
	}
	return false;
}

void mainLoop()
{
	clearScreen();
	initPlayer();
	initBall();

	while (true) {

		if (vdpStatusRegVal & VDP_FLAG_COLLISION) {
			checkCollision();
		}

		updatePlayers();
		updateBall();

		if (checkGameover()) {
			for(int i = 0; i < MAX_PLAYERS; i++) {
				vdp_disableSprite(players[i].spriteId);
			}
			vdp_disableSprite(0);
			vdp_disableSprite(ball.spriteId);
			while (getChar() != 0x0D);
			break;
		}

		vdp_waitVDPReadyInt();

		if (players[0].vdpReady) {
			vdp_setSpritePosition(players[0].spriteId, players[0].x, players[0].y);
			players[0].vdpReady = false;
		}
		if (players[1].vdpReady) {
			vdp_setSpritePosition(players[1].spriteId, players[1].x, players[1].y);
			players[1].vdpReady = false;
		}

		if (ball.vdpReady) {
			vdp_setSpritePosition(ball.spriteId, ball.x, ball.y);
			ball.vdpReady = false;
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

	mainMenu();

	vdp_enableVDPReadyInt();

	while(true) {
		vdp_waitVDPReadyInt();
		if (isKeyPressed() && getChar() == 0x0D) {
			mainLoop();

			vdp_fillScreen(0x00);

			mainMenu();
		}
	}
}

