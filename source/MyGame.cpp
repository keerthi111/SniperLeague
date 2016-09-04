// Main file :D 
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include "DrawFirstWindow.cpp"
#include "EnterYourName.cpp"
#include "HostMyGame.cpp"
#include "JoinTheGame.cpp"
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <string>
#include "Game/Main.cpp"
using namespace std;

int WindowNumber_Is = -1; 
// 0 for entering name of player
// 1 for selecting type of game
// 2 for hostGame page
// 3 for JoinGame page
//4 for EXIT
// 5 for actual Game
int flag = 0;

string PlayerId; //is never empty
string* serverDetails;
string getipAddress();
string serverPort;

int main(){
	sf::Clock clock;
	sf::Time t1 = clock.restart();
	sf::RenderWindow myWind(sf::VideoMode(1368,768),"SniperLeague");
	TypeOfGame game(myWind);
	EnterYourName PlayerName(myWind);
	HostMyGame hostGame(myWind);
	JoinTheGame joinGame(myWind);
	//sf::Music music;
	//if (!music.openFromFile("resources/audio/music.ogg")){  cout << "Cannot find resource \n";}
	//music.play();
	while(myWind.isOpen()){
		if(WindowNumber_Is == -1){
			if( (clock.getElapsedTime()).sf::Time::asSeconds() > 2){ PlayerName.setIP(getipAddress()); WindowNumber_Is++;}
			else{ sf::Texture t;
				  if(!t.loadFromFile("resources/backgrounds/FirstWindow.png")){ cout << "Cannot find resource \n";}
				  sf::Sprite firstWind;
				  firstWind.setTexture(t); 
				  myWind.draw(firstWind);
				  myWind.display();}
		}
		if( WindowNumber_Is == 0){
			PlayerName.Perform();
			if(PlayerName.ReturnId() != ""){ PlayerId = PlayerName.ReturnId(); 
												WindowNumber_Is++;}}
		if( WindowNumber_Is == 1){
			int n = game.returnTypeOfGame(); 
			if(n == 1){ hostGame.setIP(getipAddress()); WindowNumber_Is++; }
			else{ WindowNumber_Is+= n;}
		}
		if(WindowNumber_Is == 2){
			// host game
			//cout << "Host my game \n";
			hostGame.choices();
			int n = hostGame.returnState(); 
			if( n== 1 && flag == 0){ // make connections;
							//cout << "making connections \n"; 
							hostGame.setIP(getipAddress());
							serverPort = hostGame.getPort();
							char pl = (hostGame.getPlayers())[0];
							char pl2 = (hostGame.getBots())[0];
							if( (int)(pl-pl2) >1){
								string nill = "";
								char* as[5];
								string  temp[5] ={nill,serverPort, hostGame.getPlayers(),hostGame.getBots(), nill};
								for(int i = 0; i < 5; i++){ as[i] = new char[temp[i].size()+1];
													strcpy(as[i], temp[i].c_str());}

								//cout << hostGame.getPlayers() << "   "<<hostGame.getBots() << endl;
								flag = 1;
							    Main(5,as);
								//cout << "Doing connection \n";
								//Doconnection(2,as);
								//WindowNumber_Is = 5;
							}
							else{
								char* as[2];
								string one = hostGame.getBots();
								as[1] = new char[one.size()+1];
								strcpy(as[1], one.c_str());
								Main(2,as);
							}
							return 0;
							//now make the internet connection
			}
			if(n == -1 && flag==0){ WindowNumber_Is--;}
		}
		if(WindowNumber_Is == 3){
			// join Game
			//cout << "Join the game \n";
			joinGame.Perform();
			if( joinGame.returnStat()){
				            serverDetails = joinGame.ReturnId(); 
				            string newout[4] = {"",serverDetails[0],serverDetails[1],serverDetails[2]};
                            const char* ret[4];
							char* out[4];
							for(int i =0; i < 4; i++){ out[i] = new char[newout[i].size()+1];}
						    for(int i =0; i < 4; i++){
								 strcpy(out[i], newout[i].c_str());}
							//cout << serverDetails[0] << " " << serverDetails[1] << "  " << serverDetails[2] << endl;
							//myWind.close();
							//make connection with server
							Main(4,out);
							return 0;
							//WindowNumber_Is = 5;
					}
			//myWind.close();
		}
		

}
return 0;

}


string getipAddress(){
	struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;
    getifaddrs(&ifAddrStruct);
    //vector of ipaddress which is to be returned as output
    vector<string> ipaddress;
    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr){ continue; }
        if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4 is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            string temp = (string)(ifa->ifa_name);
	        if( temp == "eth0" || temp == "wlan0"){
	            	ipaddress.push_back((string)(ifa->ifa_name));
	            	ipaddress.push_back((string)(addressBuffer));}
        }}

        string IP = "";
		for(int i =0;i < ipaddress.size(); i+=2){ 
					if( ipaddress[i] == "eth0"){IP += "ETHERNET :";}
					if( ipaddress[i] == "wlan0"){IP += "WIRELESS :";}
					IP += " : "; IP+= ipaddress[i+1]; IP+= "\n";}
		return IP;}