

#include "stdafx.h"

#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include <cstdint>
#include <algorithm> 
#include <list>
#include <vector>
#include <string>
#include <iostream>
#include "Vektor2d.h"

using namespace std;


// Simulationsgeschwindigkeit
const double DT = 100.0;


const int PixelX = 800;
const int PixelY = 600;
const int SledLen = 50;
const double StartVel = 7;



class GameWindow : public Gosu::Window
{
public:
	Gosu::Image bild;
	GameWindow()
		: Window(PixelX, PixelY)
		, bild("rakete.png")
	{
		//set_caption("Gosu Tutorial Game mit Git");
	}





	class BALL {
		double vx = 0;
		double vy = 0;
		double amout = 0;

	public:
		double x = -PixelX / 2;
		double y = PixelY - 10;

		void set_vx(double x) {
			this->vx = x;
		}

		void set_vy(double y) {
			this->vy = y;
		}

		void set_amout(double a) {
			this->amout = StartVel * a;
			this->vy = sqrt(this->amout)*this->vy;
			this->vx = sqrt(this->amout)*this->vx;
		}

		double get_vx() {
			return this->vx;
		}

		double get_vy() {
			return this->vy;
		}

		double get_amout() {
			return this->amout;
		}
	};

	class SLED {
		int len = 2;
		double x;
	public:
		boolean skill;

		void set_len(int x) {
			this->len = x;
			if (len < 1) {
				len = 1;
			}
			if (len > 5) {
				len = 5;
			}
		}

		void set_x(double X) {
			this->x = X;
			if (x <= 0) {
				x = 0;
			}
			if (x >= PixelX - this->len*SledLen) {
				x = PixelX - this->len*SledLen;
			}
		}

		double get_x() {
			return this->x;
		}

		int get_len() {
			return this->len;
		}
	};

	class BLOCK {
		double x;
		double y;
		double wx;
		double wy;
		int hit;
		Gosu::Color c;

	public:
		BLOCK(double X, double Y, double wX, double wY, int h, Gosu::Color C) {
			this->x = X;
			this->y = Y;
			this->wx = wX;
			this->wy = wY;
			this->hit = h;
			this->c = C;
		}

		double get_x() {
			return this->x;
		}

		double get_y() {
			return this->y;
		}

		double get_wx() {
			return this->wx;
		}

		double get_wy() {
			return this->wy;
		}
		int get_hit() {
			return this->hit;
		}
		Gosu::Color get_color() {
			return this->c;
		}

		void red_hit() {
			if (this->hit >= 1) {
				this->hit = this->hit - 1;
			}
			else {
				this->hit = 0;
			}

		}
	};

	class LEVEL {
	public:
		int number;
		list<BLOCK> block;
	};

	BALL ball;
	SLED sled;
	double A;
	double B;
	double C;
	double D;
	double LineX;
	double LineY;
	int LevelCount = 1;
	LEVEL L1;
	enum stat { Init, Start, Play, NextLevel };
	stat status = Init;
	list<BLOCK>::iterator iter;




	// wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{

		//graphics().draw_line(10, 20, Gosu::Color::RED,200, 100, Gosu::Color::GREEN,0.0);
		//graphics().draw_triangle(x, y, Gosu::Color::RED, x+200, y+200, Gosu::Color::GREEN, x+100, y+20, Gosu::Color::BLUE, 0.0);
		//bild.draw_rot(50, 50,0.0, x, 0.5);
		graphics().draw_rect(A, PixelY - 5, B, 5, Gosu::Color::GREEN, 0.0);
		graphics().draw_rect(ball.x - 5, ball.y - 5, 10, 10, Gosu::Color::GREEN, 0.0);

		for (auto list : L1.block) {
			if (list.get_hit() > 0) {
				graphics().draw_rect(list.get_x(), list.get_y(), list.get_wx(), list.get_wy(), list.get_color(), 0.0);
			}
		}

		if (status == Start) {
			graphics().draw_line(PixelX / 2, PixelY - 10, Gosu::Color::GREEN, LineX, LineY, Gosu::Color::GREEN, 0.0);
		}
	}

	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{

		switch (status) {

		case Init:

			L1.block.push_front(BLOCK(300, 200, 200, 200, 5, Gosu::Color::RED));
			L1.block.push_front(BLOCK(100, 50, 45, 45, 2, Gosu::Color::YELLOW));
			L1.block.push_front(BLOCK(150, 50, 45, 45, 2, Gosu::Color::BLUE));
			L1.block.push_front(BLOCK(200, 50, 45, 45, 2, Gosu::Color::GRAY));
			L1.block.push_front(BLOCK(250, 50, 45, 45, 2, Gosu::Color::AQUA));
			status = Start;
			break;


		case Start:
			ball.x = PixelX / 2;
			ball.y = PixelY - 10;
			C = C + 0.1;
			LineX = PixelX / 2 + cos(C) * 50;
			LineY = PixelY - 10 - abs(sin(C)) * 50;

			if (input().down(Gosu::MS_LEFT)) {
				ball.set_amout(1);
				ball.set_vx((cos(C) + 0.1)*ball.get_amout());
				ball.set_vy(-abs((sin(C)) + 0.1)*ball.get_amout());

				status = Play;
			}

			break;

		case Play:
			ball.x = ball.x + ball.get_vx();
			ball.y = ball.y + ball.get_vy();


			for (iter = L1.block.begin(); iter != L1.block.end(); iter++) {


				/*

				if (ball.y >= *iter.get_y() && ball.y <= list.get_y() + list.get_wy() && ball.x >= list.get_x() && ball.x <= list.get_x() + list.get_wx()) {



				if (ball.x > list.get_x() + abs(ball.get_vx()) && ball.x < list.get_x() + list.get_wx() - abs(ball.get_vx())) {
				ball.set_vy(ball.get_vy()*-1);
				}
				else {
				ball.set_vx(ball.get_vx() *-1);
				}



				//if (list.get_hit() <= 0) {
				//L1.block.erase(list);
				}

				*/
			}
		}


		if (ball.y >= PixelY) {
			status = Start;
		}
		break;

		case NextLevel:
			break;
	}


	sled.set_x(input().mouse_x());
	A = sled.get_x();
	B = sled.get_len()* SledLen;


	if (ball.x <= 0 || ball.x >= PixelX) {
		ball.set_vx(ball.get_vx()*-1);
	}
	if (ball.y <= 0) {
		ball.set_vy(ball.get_vy() *-1);
	}
	if (ball.y >= PixelY - 5 && ball.x >= sled.get_x() && ball.x <= sled.get_x() + sled.get_len()*SledLen && ball.get_vy()>0) {
		ball.set_vx((1.8*(ball.x - sled.get_x()) / (sled.get_len()*SledLen) - 0.9)*ball.get_amout());
		ball.set_vy(sqrt(pow(ball.get_amout(), 2) - pow(ball.get_vx(), 2))*-1);
	}
}
};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
}

