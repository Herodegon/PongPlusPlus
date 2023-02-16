#include <iostream>
#include <vector>
#include <string>
#include "raylib.h"

#include "object.h"

enum Flag {
  WINNER,
  COLLISION
};

int main() {
  int WIDTH = 800;
  int HEIGHT = 450;
  srand(time(0));
  
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(WIDTH, HEIGHT, "Pong++");
  SetTargetFPS(60);
  
  //Create Game Objects
  Parameters playerParameters = 
  {
    WIDTH/12.0f,    //X Position
    HEIGHT/3.0f,   //Y Position
    WIDTH/50.0f,   //Width
    HEIGHT/3.0f    //Height
  };
  Paddle player1(playerParameters,{KEY_W,KEY_S});
  playerParameters.posX = WIDTH-playerParameters.posX;
  Paddle player2(playerParameters,{KEY_UP,KEY_DOWN});
  Ball ball(WIDTH/2.0f, HEIGHT/2.0f, WIDTH/80.0f);
  playerParameters.posX = WIDTH-100.0f;
  Paddle player3(playerParameters,{KEY_R,KEY_F});
  
  player1.velocity = {0.0f,250.0f};
  player2.velocity = {0.0f,250.0f};
  
  bool isGameStart = true;
  
  // Main Game Loop
  std::string flag = "";
  float flagTime = 0;
  do 
  {
    DrawFPS(10,10);
    
    /*==Experimental Feature: Window Resizing==
    if(IsWindowResized()) {
      WIDTH = GetScreenWidth();
      HEIGHT = GetScreenHeight();
    }
    */
    
    BeginDrawing();
      ClearBackground(BLACK);
      player1.Draw();
      player2.Draw();
      player3.Draw();
      ball.Draw();
      DrawText(flag.c_str(),WIDTH/2.0f,HEIGHT/4.0f,30,RAYWHITE);
    EndDrawing();
    
    if(GetTime() >= flagTime+1 && flag != "") {
      flag = "";
    }
    
    //Initialize Game 
    if(isGameStart) 
    {
      isGameStart = false;
      ball.velocity = {200.0f,200.0f};
      ball.SetRandDirection();
      ball.timeSinceGain = GetTime();
    }
    
    std::cout << "Screen Width: " << WIDTH << "\n"
            << "Screen Height: " << HEIGHT << std::endl
            << "Ball PosX: " << ball.pos.x
            << " | " << ball.obj.x << "\n"
            << "Ball PosY: " << ball.pos.y
            << " | " << ball.obj.y << std::endl
            << "Player1 PosX: " << player1.pos.x
            << " | " << player1.obj.x << "\n"
            << "Player1 PosY: " << player1.pos.y
            << " | " << player1.obj.y << std::endl;
            
    //Get Player Input
    player1.Input();
    player2.Input();
    player3.Input();
    
    //Detect Ceiling/Floor
    if(ball.obj.y < 0+(ball.obj.radius)) 
    {
      ball.obj.y = 0+(ball.obj.radius);
      ball.velocity.y*=-1;
    } 
    else if(ball.obj.y > HEIGHT-(ball.obj.radius)) 
    {
      ball.obj.y = HEIGHT-(ball.obj.radius);
      ball.velocity.y*=-1;
    }
    
    //Detect Pong
    Paddle selectedPong;
    for(int i = 0; i < 2; i++) 
    {
      switch(i) {
        case 0:
          selectedPong = player1;
          break;
        case 1:
          selectedPong = player2;
          break;
        case 2:
          selectedPong = player3;
          break;
      }
      if(CheckCollisionCircleRec(ball.pos,ball.obj.radius,selectedPong.obj)) 
      {
        flag = "!";
        flagTime = GetTime();
        ball.velocity.x*=-1;
        switch(i) 
        {
          case 0:
            ball.obj.x = (selectedPong.obj.x+selectedPong.obj.width) + ball.obj.radius;
            break;
          case 1:
            ball.obj.x = selectedPong.obj.x - ball.obj.radius;
            break;
        }
        if(rand()%6 == 0) {ball.velocity.y*=-1;}
      }
    }
    
    //Scoring System
    if(ball.obj.x < 0) 
    {
      player2.score++;
      ball.Reset();
      isGameStart = true;
      
      flag = "Winner!";
      flagTime = GetTime();
      
    }
    else if(ball.obj.x > WIDTH - (ball.obj.radius)) 
    {
      player1.score++;
      ball.Reset();
      isGameStart = true;
      
      flag = "Winner!";
      flagTime = GetTime();
    }
    
    if(ball.DidGain) {
      flag = "V++";
      flagTime = GetTime();
      ball.DidGain = false;
    }
  } while(!WindowShouldClose());
  
  CloseWindow();
  
  return 0;
}
