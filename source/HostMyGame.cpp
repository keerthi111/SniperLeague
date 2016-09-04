#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

// Players icon co-ordinates 
int PLAYERSyLow = 125;
int PLAYERSyHigh = 160;
int playerX[12] = {152,186,222,258,290,327,366,402,440,476,518,554};

// Bots icon co-ordinates
int BOTSyLow = 667;
int BOTSyHigh = 700;
int BOTX[10] = {100,130,170,200,240,270,313,343,388,420};

// Hardness level co-ordinates
//xlow ylow length width
int hardness[4][4] = { {140,265,45,15}, {260,265,145,15}, {450,265,80,15},{255,320,150,20}};

// myArena co-ordinates
//xlow ylow length width
//int myArenaLength = 95;
//int myArenaWidth = 50;
//int myArena[5][2] = { {100,435}, {240,435}, {388,437}, {100,530},{240,533}};
int start[4] = {1150,665,140,70};
int back[4] = {847,670,164,65};

class HostMyGame{
	private:
		string landscape;
		int numberOfPlayers;
		int numberOfBots;
		vector<string> currentPlayers;
		int hardnessLevel; // 0 for Novice, 1 for intermediate, 2 for Expert and 3 for SuperSniper!!!
		//int myArenaLevel; // 0 for grass, 1 for sand, 2 for chiseled_ice, 3 for ice, 4 for tiled_ground
		sf::Texture texture;
		sf::Sprite background;
		sf::RenderWindow* window;
		//sf::Music music;
		sf::Text startIt;
		sf::Font font;

		sf::Text IP;
		sf::Text portNo;

		string portnumber;
		string ipAddress;

		bool playersSelected;
		bool levelSelected;
		//bool myArenaSelected;
		bool startClicked;
		bool backClicked;
		bool AISelected;

		sf::RectangleShape highlightPlayer;
	public:
		HostMyGame(sf::RenderWindow &mywind){
			window = &mywind;
			if(!texture.loadFromFile("resources/backgrounds/ActAsHost.png")){ cout << "Cannot find resource \n";}
			background.setTexture(texture);
			if (!font.loadFromFile("resources/fonts/font.ttf")){cout << "Cannot find resource \n";}
			startIt.setFont(font); 
			startIt.setPosition(sf::Vector2f(800,200));
			startIt.setCharacterSize(20);

			IP.setFont(font);
			IP.setPosition(sf::Vector2f(760,350));
			IP.setCharacterSize(30);

			portNo.setFont(font);
			portNo.setPosition(sf::Vector2f(850,130));
			portNo.setCharacterSize(35);

			playersSelected = levelSelected  = startClicked =  AISelected= false;
			//= myArenaSelected
			//if (!music.openFromFile("resources/audio/select.ogg")){  cout << "Cannot find resource \n";}
		}
		void choices(){
			sf::Event event;
			startClicked = backClicked = false;
    		while(window->pollEvent(event)){
    			if(event.type == sf::Event::Closed){
		            window->close();}
		        if(event.type == sf::Event::KeyPressed){
		        	if(sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)){
		        	 		if(portnumber != ""){
		        	 			portnumber = portnumber.substr(0,portnumber.size()-1);}
		        	 }
		        	 if(event.text.unicode >= 26 && event.text.unicode <= 34){
		        	 		portnumber+= (char)(event.text.unicode+48-26); }
		        	 //cout << portnumber << endl;
		        	 portNo.setString(portnumber);
		        }
		        if(event.type == sf::Event::MouseButtonPressed){
		        	if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
		        		sf::Vector2i locationMouse = sf::Mouse::getPosition(*window);
		        		//checks if player number is selected
		        		if( locationMouse.y >= PLAYERSyLow && locationMouse.y <= PLAYERSyHigh){
		        			 for(int i = 0; i < 12; i+=2){
		        			 		if( locationMouse.x >= playerX[i] && locationMouse.x <= playerX[i+1]){
		        			 				//music.play(); 
		        			 				playersSelected = true; numberOfPlayers = i/2 + 1;}}}
		        		//cout << "Player selected is : " << numberOfPlayers << endl;

		        		if( locationMouse.y >= BOTSyLow && locationMouse.y <= BOTSyHigh){
		        			 for(int i = 0; i < 10; i+=2){
		        			 		if( locationMouse.x >= BOTX[i] && locationMouse.x <= BOTX[i+1]){
		        			 				//if( (i/2 + 1) < numberOfPlayers){
		        			 					numberOfBots = i/2+1; AISelected = true;
		        			 					// music.play(); 
		        			 					//}
		        			 		}}}
		        		
		        		//cout << locationMouse.x << "  " << locationMouse.y << endl;
		        		
		        		//select myArenas
		        		//for(int i =0; i <5; i++){
		        		//	if(locationMouse.y >= myArena[i][1] && locationMouse.y <= (myArena[i][1]+myArenaWidth )){
		        			//	if(locationMouse.x >= myArena[i][0] && locationMouse.x <= (myArena[i][0]+myArenaLength)){
		        		//			 //music.play(); 
		        					// myArenaSelected= true; myArenaLevel=i+1;}}}

		        		//select start
							if(locationMouse.y >= start[1] && locationMouse.y <= start[1]+start[3]){
		        				if(locationMouse.x >= start[0] && locationMouse.x <= start[0]+start[2]){
		        					 if( (playersSelected) && (AISelected && (portnumber.size()>0))){startClicked= true; 
		        					 	//music.play();
		        					 }}}
		        		
		        		//back selected
							if(locationMouse.y >= back[1] && locationMouse.y <= back[1]+back[3]){
		        				if(locationMouse.x >= back[0] && locationMouse.x <= back[0]+back[2]){
		        					 if(!startClicked){backClicked= true; cout<<"back clicked \n"; //music.play();
		        					}}}
		        		}}}

		        Draw();}
		void Draw(){
			window->clear();
			window->draw(background);
			window->draw(startIt);
			window->draw(portNo);
			window->draw(IP);
			if(playersSelected){
				highlightPlayer.setPosition(sf::Vector2f(playerX[2*(numberOfPlayers-1)]-5,PLAYERSyLow-5));
				highlightPlayer.setSize(sf::Vector2f(46,45));
				highlightPlayer.setOutlineThickness(3.0f);
				highlightPlayer.setFillColor( sf::Color::Transparent);
				highlightPlayer.setOutlineColor(sf::Color::Red);
				window->draw(highlightPlayer);
			}
			if(AISelected){
				highlightPlayer.setPosition(sf::Vector2f(BOTX[2*(numberOfBots-1)]-10,BOTSyLow-5));
				highlightPlayer.setSize(sf::Vector2f(50,45));
				highlightPlayer.setOutlineThickness(3.0f);
				highlightPlayer.setFillColor( sf::Color::Transparent);
				highlightPlayer.setOutlineColor(sf::Color::Green);
				window->draw(highlightPlayer);
			}
			if(levelSelected){
				highlightPlayer.setPosition(sf::Vector2f(hardness[hardnessLevel-1][0]-5,hardness[hardnessLevel-1][1]-5));
				highlightPlayer.setSize(sf::Vector2f(hardness[hardnessLevel-1][2]+20,hardness[hardnessLevel-1][3]+12  ));
				highlightPlayer.setOutlineThickness(3.0f);
				highlightPlayer.setFillColor( sf::Color::Transparent);
				highlightPlayer.setOutlineColor(sf::Color::White);
				window->draw(highlightPlayer);
			}
			//if(myArenaSelected){
			//	highlightPlayer.setPosition(sf::Vector2f(myArena[myArenaLevel-1][0]-5,myArena[myArenaLevel-1][1]-5));
			///	highlightPlayer.setSize(sf::Vector2f(myArenaLength+ 15,myArenaWidth+12  ));
			//	highlightPlayer.setOutlineThickness(3.0f);
			//	highlightPlayer.setFillColor( sf::Color::Transparent);
			//	highlightPlayer.setOutlineColor(sf::Color::Blue);
			//	window->draw(highlightPlayer);
			//}
			if(startClicked){
				startIt.setString("Wait for connections to be made");
				//cout << "Start Clicked \n";

			}
			window->display(); }
		void setIP(string v){ ipAddress = v; IP.setString(v); }
		string getPort(){return portnumber;}
		string getPlayers(){
			string c; c+= char(numberOfPlayers+48); return c;}
		string getBots(){ string c; c+= (char)(numberOfBots +47); return c;}
		int returnState(){ if(backClicked){return -1;} else if(startClicked){return 1;} else{return 0;}}
};
