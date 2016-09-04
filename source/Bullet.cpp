#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
using namespace std;

class Bullet{
    public:

       Bullet();
       void update(sf::Time deltaTime);
       sf::CircleShape return_shape();
       void shoot();
       void setPosition(sf::Vector2f position);
       void setDirection(float angle);
       //void getDirection();
       sf::Vector2f getPosition();
       void moveY(float deltaY);
       bool getFiringStatus();
       bool outOfBounds();

    private:

        sf::CircleShape bullet;
        sf::Texture explosion;
        sf::SoundBuffer shootingEffect;
        sf::Sound shot;

        bool canFire = true;
        bool outBounds = false;
        float BulletSpeed;};

Bullet::Bullet():bullet(),shootingEffect(),shot()
{     if(!shootingEffect.loadFromFile("resources/NovaShot.wav")){
            std::cout << "File not found" << std::endl;}
      shot.setBuffer(shootingEffect);
      bullet.setFillColor(sf::Color::White);
      bullet.setRadius(2.f);}

void Bullet::setPosition(sf::Vector2f position){ bullet.setPosition(position); }
sf::Vector2f Bullet::getPosition(){ return bullet.getPosition();}
void Bullet::setDirection(float angle){ bullet.setRotation(angle); }
void Bullet::moveY(float deltaY){ BulletSpeed = deltaY; }
bool Bullet::getFiringStatus(){ return canFire; }
void Bullet::update(sf::Time deltaTime){
     sf::Vector2f movement(0.f,0.f);
     if( (bullet.getPosition().y <=0 || bullet.getPosition().y >= 768) || (bullet.getPosition().x <=0 || bullet.getPosition().x >= 1368)){
        BulletSpeed = 0;
        canFire = true;
        outBounds = true;}
     else{
        canFire = false;
        movement.y -= sin( (bullet.getRotation())*3.14/180 )*BulletSpeed;
        movement.x -= cos( (bullet.getRotation())*3.14/180 )*BulletSpeed; //acording to direction
        bullet.move(movement * deltaTime.asSeconds());}}
bool Bullet::outOfBounds(){ return outBounds;}
void Bullet::shoot(){ shot.play();}
sf::CircleShape Bullet::return_shape(){ return bullet;}

