#include "GamePlay.hpp"

#include <SFML/Window/Event.hpp>
#include <stdlib.h>
#include <time.h>

GamePlay::GamePlay(std::shared_ptr<Context> &context) : mContext(context), mSnakeDirection({16.f, 0.f}), mElapsedTime(sf::Time::Zero)
{
     srand(time(nullptr));
}

GamePlay::~GamePlay()
{
}

void GamePlay::Init()
{
     //Load assets
     mContext->mAssets->addTexture(GRASS, "assets/textures/grass.png", true);
     mContext->mAssets->addTexture(FOOD, "assets/textures/food.png");
     mContext->mAssets->addTexture(WALL, "assets/textures/wall.png", true);
     mContext->mAssets->addTexture(SNAKE, "assets/textures/snake.png");

     //Grass
     mGrass.setTexture(mContext->mAssets->getTexture(GRASS));
     mGrass.setTextureRect(mContext->mWindow->getViewport(mContext->mWindow->getDefaultView()));

     //Wall
     for (auto &wall : mWalls)
     {
          wall.setTexture(mContext->mAssets->getTexture(WALL));
     }

     //x wall position
     mWalls[0].setTextureRect({0, 0, (int)mContext->mWindow->getSize().x, 16});
     mWalls[1].setTextureRect({0, 0, (int)mContext->mWindow->getSize().x, 16});
     mWalls[1].setPosition(0, mContext->mWindow->getSize().y - 16);

     //y wall position
     mWalls[2].setTextureRect({0, 0, 16, (int)mContext->mWindow->getSize().y});
     mWalls[3].setTextureRect({0, 0, 16, (int)mContext->mWindow->getSize().y});
     mWalls[3].setPosition(mContext->mWindow->getSize().x - 16, 0);

     //Food
     mFood.setTexture(mContext->mAssets->getTexture(FOOD));
     mFood.setPosition(mContext->mWindow->getSize().x / 2, mContext->mWindow->getSize().y / 2);

     //Snake
     mSnake.Init(mContext->mAssets->getTexture(SNAKE));
}

void GamePlay::ProcessInput()
{
     sf::Event event;

     while (mContext->mWindow->pollEvent(event))
     {
          if (event.type == sf::Event::Closed)
          {
               mContext->mWindow->close();
          }

          else if (event.type == sf::Event::KeyPressed)
          {
               sf::Vector2f newDirection = mSnakeDirection;

               switch (event.key.code)
               {
               case sf::Keyboard::Up:
                    newDirection = {0.f, -16.f};
                    break;

               case sf::Keyboard::Down:
                    newDirection = {0.f, 16.f};
                    break;

               case sf::Keyboard::Left:
                    newDirection = {-16.f, 0.f};
                    break;

               case sf::Keyboard::Right:
                    newDirection = {16.f, 0.f};
                    break;

               default:
                    break;
               }

               if (std::abs(mSnakeDirection.x) != std::abs(newDirection.y) || std::abs(mSnakeDirection.y) != std::abs(newDirection.y))
               {
                    (mSnakeDirection = newDirection);
               }
          }
     }
}

void GamePlay::Update(sf::Time deltaTime)
{
     (mElapsedTime += deltaTime);

     if (mElapsedTime.asSeconds() > 0.1)
     {
          for (auto &wall : mWalls)
          {
               //Go to GameOver states
               if (mSnake.isOn(wall))
               {
                    break;
               }
          }

          if (mSnake.isOn(mFood))
          {
               mSnake.Grow(mSnakeDirection);

               int x = 0, y = 0;

               x = std::clamp<size_t>(rand() % mContext->mWindow->getSize().x, 16, mContext->mWindow->getSize().x - 2 * 16);
               x = std::clamp<size_t>(rand() % mContext->mWindow->getSize().y, 16, mContext->mWindow->getSize().y - 2 * 16);

               mFood.setPosition(x, y);
          }

          else
          {
               mSnake.Move(mSnakeDirection);
          }

          (mElapsedTime = sf::Time::Zero);
     }
}

void GamePlay::Draw()
{
     mContext->mWindow->clear();

     mContext->mWindow->draw(mGrass);

     for (auto &wall : mWalls)
     {
          mContext->mWindow->draw(wall);
     }

     mContext->mWindow->draw(mFood);
     mContext->mWindow->draw(mSnake);

     mContext->mWindow->display();
}

void GamePlay::Pause()
{
}

void GamePlay::Start()
{
}