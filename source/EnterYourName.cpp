//Asks user to enter his/her name
//User proceeds to next page only if he enters non empty string as input
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class EnterYourName{
	private:
		string PlayerId;
		sf::RenderWindow* window;
		bool ExitThisWindow;
		sf::Texture texture;
		sf::Sprite background;
		sf::Text text;
		sf::Font font;
		sf::Text ipAdd;
		sf::Font fontForIP;
	public:
		EnterYourName( sf::RenderWindow &wind){ 
			window= &wind; 
			PlayerId=""; 
			ExitThisWindow=false;
			if(!texture.loadFromFile("resources/backgrounds/PlayerName.png")){
				cout << "Cannot find resource \n";}
		    if (!font.loadFromFile("resources/fonts/font.ttf")){cout << "Cannot find resource \n";}
		    texture.setSmooth(true);
		    text.setString(PlayerId);
		    text.setFont(font);   text.setCharacterSize(30);
		    ipAdd.setFont(font);  ipAdd.setCharacterSize(30);
		    text.setPosition(sf::Vector2f(350,295));
		    ipAdd.setPosition(sf::Vector2f(550,530));
		    text.setColor(sf::Color::Red);    ipAdd.setColor(sf::Color::Red);
			background.setTexture(texture);}

		void setIP(string IP){ ipAdd.setString(IP);}
		void Perform(){
			sf::Event event;
    		while(window->pollEvent(event)){
    			if(event.type == sf::Event::Closed){
		            window->close();}
		        if(event.type == sf::Event::KeyPressed){
		        	 if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return)){
		        	 		if(PlayerId != ""){ExitThisWindow = true;}}
		        	 if(sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)){
		        	 		if(PlayerId != ""){
		        	 			PlayerId = PlayerId.substr(0,PlayerId.size()-1);}
		        	 }
		        	 else if(event.text.unicode >= 0 && event.text.unicode <= 25){
		        	 		PlayerId+= (char)(event.text.unicode+65); }
		        	 else if(event.text.unicode >= 26 && event.text.unicode <= 34){
		        	 		PlayerId+= (char)(event.text.unicode+48-26); }
		        }}
		        text.setString(PlayerId);
		    	Draw();
		 }
		void Draw(){
			 window->clear();
			 window->draw(background);
			 window->draw(text);
			 window->draw(ipAdd);
   			 window->display();}
		string ReturnId(){ 
			if(ExitThisWindow){return PlayerId;}
			else{ return "";}}
};


/*
Coordinates for ip address
339   531
1058   531
346   701
1056   700


Coordinates for player name
351   309
1044   311
*/