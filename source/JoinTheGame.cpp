//Asks user to enter details of server
//User proceeds to next page only if he enters non empty string as input
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
using namespace std;

class JoinTheGame{
	private:
		string details[3]; //ipAddress portNumber UrPort
		int currentInput;

		sf::RenderWindow* window;
		bool DONE;
		sf::Texture texture;
		sf::Sprite background;
		sf::Font font;
		sf::Text ipAdd;
		sf::Text portNum;
		sf::Text UserportNum;
		
	public:
		JoinTheGame( sf::RenderWindow &wind){ 
			window= &wind; 
			DONE =false;
			currentInput = 0;
			if(!texture.loadFromFile("resources/backgrounds/JoinTheGame.png")){
				cout << "Cannot find resource \n";}
		    if (!font.loadFromFile("resources/fonts/font.ttf")){cout << "Cannot find resource \n";}
		    texture.setSmooth(true);
		    
		    ipAdd.setFont(font);  ipAdd.setCharacterSize(20);
		    portNum.setFont(font);  portNum.setCharacterSize(20);
		    UserportNum.setFont(font);  UserportNum.setCharacterSize(20);
		    
		    ipAdd.setPosition(sf::Vector2f(440,220));
		    portNum.setPosition(sf::Vector2f(440,280));
		    UserportNum.setPosition(sf::Vector2f(440,330));
		    
		    ipAdd.setColor(sf::Color::Blue);
		    portNum.setColor(sf::Color::Blue);
		    UserportNum.setColor(sf::Color::Blue);

		    background.setTexture(texture);}
		void Perform(){
			sf::Event event;
    		while(window->pollEvent(event)){
    			if(event.type == sf::Event::Closed){
		            window->close();}
		        if(event.type == sf::Event::KeyPressed){
		        	 if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return)){ 
		        	 		if(details[0] != "" && (details[1] != "" && details[2] != "")){DONE =true;}}
		        	 if(sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)){ currentInput = (currentInput+1)%3;}
		        	 if(sf::Keyboard::isKeyPressed(sf::Keyboard::Period)){ 
		        	 		details[currentInput]+= ".";}
		        	 if(sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)){
		        	 	 if(details[currentInput] != ""){
		        	 			details[currentInput] =details[currentInput].substr(0,(details[currentInput].size())-1);}
		        	 }
		        	 else if(event.text.unicode >= 0 && event.text.unicode <= 25){
		        	 		details[currentInput]+= (char)(event.text.unicode+65); }
		        	 else if(event.text.unicode >= 26 && event.text.unicode <= 34){
		        	 		details[currentInput]+= (char)(event.text.unicode+48-26); }
		        }

		        ipAdd.setString(details[0]);
		        portNum.setString(details[1]);
		        UserportNum.setString(details[2]);
		    	Draw();
		 }}
		void Draw(){
			 window->clear();
			 window->draw(background);
			 window->draw(portNum);
			 window->draw(UserportNum);
			 window->draw(ipAdd);
   			 window->display();}
		string* ReturnId(){ return details;}
		bool returnStat(){ return DONE;}
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