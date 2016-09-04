//Asks user to select his choice of game ie. does he wants to start his own game or does he wants to conenct to other users already playing this game
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

int XLow = 325;
int XHigh = 1000;
int HostGameButton_YLow = 333;
int HostGameButton_YHigh = 390;
int JoinGameButton_YLow = 408;
int JoinGameButton_YHigh = 470;
int ExitGameButton_YLow = 560;
int ExitGameButton_YHigh = 620;

class TypeOfGame{
	public:
		TypeOfGame(sf::RenderWindow &wind){
			window = &wind;
			hostGame = false; 
			joinGame = false;
			exitGame = false;
			if(!texture.loadFromFile("resources/backgrounds/SelectGameType.png")){
				cout << "Cannot find resource \n";}
			texture.setSmooth(true);
			background.setTexture(texture);}
		int returnTypeOfGame(){ 
			//returns 0 if none selected yet
			// returns 1 if user will host the game
			// returns 2 if user will join the game
			hostGame = joinGame = false;
			sf::Event event;
    		while(window->pollEvent(event)){
    			if(event.type == sf::Event::Closed){
		            window->close();}
		        if(event.type == sf::Event::MouseButtonPressed){
		        	if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
		        		sf::Vector2i locationMouse = sf::Mouse::getPosition(*window);
		        		if( locationMouse.x >= XLow && locationMouse.x <= XHigh){
		        				        			if( locationMouse.y >= HostGameButton_YLow && locationMouse.y <= HostGameButton_YHigh){hostGame= true;}
		        				        			else if( locationMouse.y >= JoinGameButton_YLow && locationMouse.y <= JoinGameButton_YHigh){joinGame= true;}
		        				        			else if( locationMouse.y >= ExitGameButton_YLow && locationMouse.y <= ExitGameButton_YHigh){ exitGame= true;}
		        				        		}
		        		
		        }}}

			if(  (!hostGame && !joinGame ) && !exitGame){ drawWindow(); return 0;}
			else if( hostGame){ hostGame =false; return 1;}
			else if( joinGame){ joinGame = false; return 2;}
			else if( exitGame){ exitGame =false; return 3;}}
		void drawWindow(){

			 window->clear();
			 window->draw(background);
   			 window->display();}	
	private:
		bool hostGame;
		bool joinGame;
		bool exitGame;
		sf::Texture texture;
		sf::Sprite background;
		sf::RenderWindow* window; };
