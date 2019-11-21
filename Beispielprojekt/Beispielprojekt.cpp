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


const double DT = 100.0; // Simulationsgeschwindigkeit
const int PixelX = 800;//Fensterabmessungen
const int PixelY = 600;
const int SledLen = 50;//Schlittenlänge einer Schlitteneinheit
const double StartVel = 7;//Grund bzw Startgeschwindigkeit des Balls



class GameWindow : public Gosu::Window {
public:
	GameWindow() : Window(PixelX, PixelY)
	{
		set_caption("Schaeffler Shooter");
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

	public:
		BLOCK(double X, double Y, double wX, double wY, int h) {
			this->x = X * 50;
			this->y = Y * 50 + 16;
			this->wx = wX * 50;
			this->wy = wY * 50;
			this->hit = h;
		}

		double get_x() {
			return this->x;
		}
		double get_y() {
			return this->y;
		}
		double get_y2() {
			return this->y + this->wy;
		}
		double get_x2() {
			return this->x + this->wx;
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
		void red_hit() {
			if (this->hit >= 1) {
				this->hit = this->hit - 1;
			}
			else {
				this->hit = 0;
			}
		}
	};


	BALL ball;
	SLED sled;
	list<BLOCK> level;


	int LevelCount = 0, life = 10;
	double StartLineX, StartLineY, C;
	list<BLOCK>::iterator iter, iter_erase;
	bool hit = false;
	int wait = 2;

	enum stat { Init, Start, Play };
	stat status = Init;

	Gosu::Color ColorList[10] = { Gosu::Color::WHITE,Gosu::Color::RED,Gosu::Color::YELLOW,Gosu::Color::GREEN,Gosu::Color::CYAN,Gosu::Color::BLUE,Gosu::Color::FUCHSIA,Gosu::Color::GRAY };


	void LevelSetup(int nr) {//Aufbau der Level

		//Aufbau:
		//level.push_front(BLOCK(x-pos,y-pos,breite,höhe,Anzahl_Treffer));

		switch (nr) {

		case 0:
			//Level 1
			level.push_front(BLOCK(0, 0, 4, 4, 5));
			level.push_front(BLOCK(4, 0, 4, 4, 6));
			level.push_front(BLOCK(8, 0, 4, 4, 5));
			level.push_front(BLOCK(12, 0, 4, 4, 6));
			level.push_front(BLOCK(0, 4, 4, 4, 4));
			level.push_front(BLOCK(4, 4, 4, 4, 3));
			level.push_front(BLOCK(8, 4, 4, 4, 4));
			level.push_front(BLOCK(12, 4, 4, 4, 3));
			break;

		case 1:
			//Level 2
			level.push_front(BLOCK(1, 1, 2, 2, 3));
			level.push_front(BLOCK(3, 1, 2, 2, 3));
			level.push_front(BLOCK(5, 1, 2, 2, 3));
			level.push_front(BLOCK(7, 1, 2, 2, 3));
			level.push_front(BLOCK(9, 1, 2, 2, 3));
			level.push_front(BLOCK(11, 1, 2, 2, 3));
			level.push_front(BLOCK(13, 1, 2, 2, 3));

			level.push_front(BLOCK(1, 3.5, 2, 2, 2));
			level.push_front(BLOCK(3, 3.5, 2, 2, 2));
			level.push_front(BLOCK(5, 3.5, 2, 2, 2));
			level.push_front(BLOCK(7, 3.5, 2, 2, 2));
			level.push_front(BLOCK(9, 3.5, 2, 2, 2));
			level.push_front(BLOCK(11, 3.5, 2, 2, 2));
			level.push_front(BLOCK(13, 3.5, 2, 2, 2));

			level.push_front(BLOCK(1, 6, 2, 2, 1));
			level.push_front(BLOCK(3, 6, 2, 2, 1));
			level.push_front(BLOCK(5, 6, 2, 2, 1));
			level.push_front(BLOCK(7, 6, 2, 2, 1));
			level.push_front(BLOCK(9, 6, 2, 2, 1));
			level.push_front(BLOCK(11, 6, 2, 2, 1));
			level.push_front(BLOCK(13, 6, 2, 2, 1));
			break;

		case 2:
			//Level 3
			level.push_front(BLOCK(1, 1, 2, 2, 1));
			level.push_front(BLOCK(3, 3, 2, 2, 1));
			level.push_front(BLOCK(5, 5, 2, 2, 1));

			level.push_front(BLOCK(3, 1, 2, 2, 2));
			level.push_front(BLOCK(5, 3, 2, 2, 2));
			level.push_front(BLOCK(7, 5, 2, 2, 2));

			level.push_front(BLOCK(5, 1, 2, 2, 3));
			level.push_front(BLOCK(7, 3, 2, 2, 3));
			level.push_front(BLOCK(9, 5, 2, 2, 3));

			level.push_front(BLOCK(7, 1, 2, 2, 4));
			level.push_front(BLOCK(9, 3, 2, 2, 4));
			level.push_front(BLOCK(11, 5, 2, 2, 4));

			level.push_front(BLOCK(9, 1, 2, 2, 5));
			level.push_front(BLOCK(11, 3, 2, 2, 5));
			level.push_front(BLOCK(13, 5, 2, 2, 5));
			break;
			
		case 3:
			//Level 4
			level.push_front(BLOCK(14, 6, 2, 2, 6));
			level.push_front(BLOCK(14, 4, 2, 2, 5));
			level.push_front(BLOCK(14, 2, 2, 2, 4));
			level.push_front(BLOCK(14, 0, 2, 2, 3));
			level.push_front(BLOCK(12, 6, 2, 2, 6));
			level.push_front(BLOCK(12, 4, 2, 2, 4));
			level.push_front(BLOCK(12, 2, 2, 2, 3));
			level.push_front(BLOCK(12, 0, 2, 2, 2));
			level.push_front(BLOCK(8, 6, 2, 2, 6));
			level.push_front(BLOCK(8, 4, 2, 2, 5));
			level.push_front(BLOCK(8, 2, 2, 2, 5));
			level.push_front(BLOCK(8, 0, 2, 2, 5));
			level.push_front(BLOCK(6, 6, 2, 2, 6));
			level.push_front(BLOCK(6, 4, 2, 2, 5));
			level.push_front(BLOCK(6, 2, 2, 2, 5));
			level.push_front(BLOCK(6, 0, 2, 2, 5));
			level.push_front(BLOCK(2, 6, 2, 2, 6));
			level.push_front(BLOCK(2, 4, 2, 2, 4));
			level.push_front(BLOCK(2, 2, 2, 2, 3));
			level.push_front(BLOCK(2, 0, 2, 2, 2));
			level.push_front(BLOCK(0, 6, 2, 2, 6));
			level.push_front(BLOCK(0, 4, 2, 2, 5));
			level.push_front(BLOCK(0, 2, 2, 2, 4));
			level.push_front(BLOCK(0, 0, 2, 2, 3));
			level.push_front(BLOCK(10, 6, 2, 2, 6));
			level.push_front(BLOCK(4, 6, 2, 2, 6));

			break;
			case 4:
			//Level 5
			level.push_front(BLOCK(6, 6, 1, 1, 5));
			level.push_front(BLOCK(5, 5, 1, 1, 5));
			level.push_front(BLOCK(4, 4, 1, 1, 5));
			level.push_front(BLOCK(7, 7, 1, 1, 5));
			level.push_front(BLOCK(8, 7, 1, 1, 5));
			level.push_front(BLOCK(8, 6, 1, 1, 5));
			level.push_front(BLOCK(8, 5, 1, 1, 5));
			level.push_front(BLOCK(8, 4, 1, 1, 5));
			level.push_front(BLOCK(9, 7, 1, 1, 5));
			level.push_front(BLOCK(10, 6, 1, 1, 5));
			level.push_front(BLOCK(11, 5, 1, 1, 5));
			level.push_front(BLOCK(12, 4, 1, 1, 5));
			level.push_front(BLOCK(12, 3, 1, 1, 5));
			level.push_front(BLOCK(11, 3, 1, 1, 5));
			level.push_front(BLOCK(10, 3, 1, 1, 5));
			level.push_front(BLOCK(9, 3, 1, 1, 5));
			level.push_front(BLOCK(8, 3, 1, 1, 5));
			level.push_front(BLOCK(7, 3, 1, 1, 5));
			level.push_front(BLOCK(6, 3, 1, 1, 5));
			level.push_front(BLOCK(5, 3, 1, 1, 5));
			level.push_front(BLOCK(4, 3, 1, 1, 5));
			level.push_front(BLOCK(0, 4, 1, 1, 5));
			level.push_front(BLOCK(1, 4, 1, 1, 5));
			level.push_front(BLOCK(2, 4, 1, 1, 5));
			level.push_front(BLOCK(3, 4, 1, 1, 5));
			level.push_front(BLOCK(12, 4, 1, 1, 5));
			level.push_front(BLOCK(13, 4, 1, 1, 5));
			level.push_front(BLOCK(14, 4, 1, 1, 5));
			level.push_front(BLOCK(15, 4, 1, 1, 5));
			level.push_front(BLOCK(6, 0, 1, 1, 5));
			level.push_front(BLOCK(6, 1, 1, 1, 5));
			level.push_front(BLOCK(6, 2, 1, 1, 5));
			level.push_front(BLOCK(10, 0, 1, 1, 5));
			level.push_front(BLOCK(10, 1, 1, 1, 5));
			level.push_front(BLOCK(10, 2, 1, 1, 5));
			level.push_front(BLOCK(11, 6, 1, 1, 5));
			level.push_front(BLOCK(12, 7, 1, 1, 5));
			level.push_front(BLOCK(13, 8, 1, 1, 5));
			level.push_front(BLOCK(14, 9, 1, 1, 5));
			level.push_front(BLOCK(2, 9, 1, 1, 5));
			level.push_front(BLOCK(3, 8, 1, 1, 5));
			level.push_front(BLOCK(4, 7, 1, 1, 5));
			level.push_front(BLOCK(5, 6, 1, 1, 5));
			
			break;
		case 5:
			//Level 6
			level.push_front(BLOCK(2, 4, 1, 1, 5));
			level.push_front(BLOCK(2, 5, 1, 1, 5));
			level.push_front(BLOCK(2, 6, 1, 1, 5));
			level.push_front(BLOCK(3, 6, 1, 1, 5));
			level.push_front(BLOCK(4, 5, 1, 1, 5));
			level.push_front(BLOCK(4, 6, 1, 1, 5));
			level.push_front(BLOCK(5, 6, 1, 1, 5));
			level.push_front(BLOCK(6, 4, 1, 1, 5));
			level.push_front(BLOCK(6, 6, 1, 1, 5));
			level.push_front(BLOCK(6, 5, 1, 1, 5));
			

			level.push_front(BLOCK(8, 4, 1, 1, 5));
			level.push_front(BLOCK(8, 5, 1, 1, 5));
			level.push_front(BLOCK(8, 6, 1, 1, 5));

			level.push_front(BLOCK(10, 6, 1, 1, 5));
			level.push_front(BLOCK(10, 4, 1, 1, 5));
			level.push_front(BLOCK(10, 5, 1, 1, 5));
			level.push_front(BLOCK(11, 4, 1, 1, 5));
			level.push_front(BLOCK(12, 4, 1, 1, 5));
			level.push_front(BLOCK(12, 5, 1, 1, 5));
			level.push_front(BLOCK(12, 6, 1, 1, 5));
			break;


		}
	}


	void draw() override {


		graphics().draw_rect(0, 0, PixelX, 16, Gosu::Color::GREEN, 0.0);//Balken oben
		graphics().draw_rect(sled.get_x(), PixelY - 5, sled.get_len()* SledLen, 5, Gosu::Color::GREEN, 0.0);//Schlitten zeichnen
		graphics().draw_rect(ball.x - 5, ball.y - 5, 10, 10, Gosu::Color::FUCHSIA, 0.0);//Ball zeichnen

		for (auto i = 0; i <= life - 2; i++) {
			graphics().draw_rect(5 + i * 15, 3, 10, 10, Gosu::Color::FUCHSIA, 0.0);//Leben zeichnen

		}


		for (auto i : level) {
			graphics().draw_rect(i.get_x(), i.get_y(), i.get_wx(), i.get_wy(), ColorList[i.get_hit()], 0.0);//Blöcke aus Blockliste zeichenen		
		}

		if (status == Start) {
			graphics().draw_line(PixelX / 2, PixelY - 10, Gosu::Color::GREEN, StartLineX, StartLineY, Gosu::Color::GREEN, 0.0);//Linie für Start zeichnen
		}
	}

	void update() override {

		switch (status) {//Spiellogik je nach Status ausführen

		case Init: {
			LevelSetup(LevelCount);//Blöcke je nach Level in Liste laden
			status = Start;
			break;
		}

		case Start: {
			ball.x = PixelX / 2;//Ball auf Grundposition
			ball.y = PixelY - 10;

			C = C + 0.1;//Laufvariable für Kreisbewegung
			StartLineX = PixelX / 2 + cos(C) * 50;//Berechnung Punkt für Linie
			StartLineY = PixelY - 10 - abs(sin(C)) * 50;

			if (input().down(Gosu::MS_LEFT)) {
				ball.set_amout(1);//Geschwindigkeit auf einmal StartVel setzen
				ball.set_vx((cos(C))*ball.get_amout()*0.7);//vy je nach Winkel setzen
				ball.set_vy(-1 * sqrt(ball.get_amout()*ball.get_amout() - ball.get_vx()*ball.get_vx()));//vy nach vx berechnen
				status = Play;
			}
			break;
		}

		case Play: {

			ball.x = ball.x + ball.get_vx();//Ball auf neue Position
			ball.y = ball.y + ball.get_vy();
			wait++;


			for (iter = level.begin(); iter != level.end(); iter++) {//Schleife über Blöcke

				if (ball.y >= iter->get_y() && ball.y <= iter->get_y2() && ball.x >= iter->get_x() && ball.x <= iter->get_x2() && wait > 2) {//Prüfung auf Treffer
					wait = 0;
					iter->red_hit();//Hit-Couter reduzieren

					if (iter->get_hit() <= 0) {//Merker um Block später zu löschen
						hit = true;
						iter_erase = iter;
					}
					if (ball.x > iter->get_x() + abs(ball.get_vx()) && ball.x < iter->get_x2() - abs(ball.get_vx())) {//Prüfen wo Ball am Block aufkommt und entsprechend abprallen lassen
						ball.set_vy(ball.get_vy()*-1);
					}
					else {
						ball.set_vx(ball.get_vx() *-1);
					}
				}
			}

			if (hit) {//Block löschen falls Treffer vorhanden
				level.erase(iter_erase);
				hit = false;
			}

			if (ball.y >= PixelY) {//Leben reduzieren und zurück zum Start falls Ball verloren
				status = Start;
				life = life - 1;
				if (life <= 0) {//Spiel rücksetzen falls keine Leben mehr
					life = 10;
					LevelCount = 0;
					status = Init;
				}
			}

			if (level.size() == 0) {//nächstes Level falls keine Blöcke mehr
				LevelCount++;
				status = Init;
			}
		}
				   break;
		}


		sled.set_x(input().mouse_x());//Mausposition für Schlitten abfragen

		if (ball.x <= 0 || ball.x >= PixelX) {//Ball rechts und links abprallen
			ball.set_vx(ball.get_vx()*-1);
		}
		if (ball.y <= 16) {//Ball oben abprallen
			ball.set_vy(ball.get_vy() *-1);
		}
		if (ball.y >= PixelY - 5 && ball.x >= sled.get_x() && ball.x <= sled.get_x() + sled.get_len()*SledLen && ball.get_vy() > 0) {//Ball am Schlitten abprallen lassen
			ball.set_vx((1.8*(ball.x - sled.get_x()) / (sled.get_len()*SledLen) - 0.9)*ball.get_amout());//Betragsfunktion für vx um Abprallwinkel zu erzwugen
			ball.set_vy(sqrt(pow(ball.get_amout(), 2) - pow(ball.get_vx(), 2))*-1);//vy nach vx anpassen
		}
	}
};

// C++ Hauptprogramm
int main() {
	GameWindow window;
	window.show();
}