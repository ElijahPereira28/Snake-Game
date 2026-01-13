#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;

Color blue= {173, 216, 230, 255};
Color darkBlue= {0, 0, 139, 255};

int cellSize= 30;
int cellCount= 25; //No. of cells for each row & column

double lastUpdateTime= 0;

bool ElementInDeque(Vector2 element, deque<Vector2> deque){ //returns true if element in deque

    for(unsigned int i = 0; i < deque.size(); i++) {
        if(Vector2Equals(deque [i], element)){
            return true;   
        }
    }
    return false;
};

bool eventTriggered(double interval){

    double currentTime= GetTime();
    if(currentTime- lastUpdateTime >= interval){
        lastUpdateTime= currentTime;
        return true;
    }else{
        return false;
    }
}

class Snake{
    public :
        deque<Vector2> body= {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
        Vector2 direction= {1,0};


        void Draw(){
            for(unsigned int i=0; i< body.size(); i++){
                float x = body[i].x;
                float y = body[i].y;
                Rectangle segment= Rectangle{x*cellSize, y*cellSize, (float)cellSize, (float)cellSize};
                DrawRectangleRounded(segment, 0.5, 1, DARKGREEN);
            }
        }

        void Update(){
            body.pop_back(); //method of deque to remove last
            body.push_front(Vector2Add(body[0], direction));

        }
};

class Food{
    
public:
    Vector2 position; //raylib offers Vector2 struct: (x,y) pos of food
    Texture2D texture;

    Food(deque<Vector2> snakeBody){ //Constructor
        Image image= LoadImage("Graphics/food.png"); //returns the pixel data of the image which can be used to create a texture
        ImageResize(&image, cellSize, cellSize); //&image--> pointer address to that struct
        texture= LoadTextureFromImage(image);
        UnloadImage(image); //Freeing some memory
        position= GenerateRandomPos(snakeBody); // calling method for random pos of food

    }

    ~Food(){ //Destructor
        UnloadTexture(texture); // frees the texture when the food object is destroyed otherwise it stays in memory and causes a memory leak.
    }
    
    void Draw(){
        DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE); //tint is optional param which allows a color filter to the texture, default=white
    }
    
    Vector2 GenerateRandomCell(){
        float x= GetRandomValue(0, cellCount-1); //Each row has max 25 cells
        float y= GetRandomValue(0, cellCount-1);
        return Vector2{x,y};
    }

    Vector2 GenerateRandomPos(deque<Vector2> snakeBody){ //random pos for food
    
        Vector2 position = GenerateRandomCell();
        while(ElementInDeque(position, snakeBody)){
            position= GenerateRandomCell();
        }
        return position;
    }
};

class Game{
    public:
    Snake snake= Snake();
    Food food= Food(snake.body);

    void Draw(){
        food.Draw();
        snake.Draw();
    }

    void Update(){
        snake.Update();
        checkCollisionWithFood();
    }

    void checkCollisionWithFood(){
        if( Vector2Equals(snake.body[0], food.position)){ //check if head of the snake and food are the same
            food.position= food.GenerateRandomPos(snake.body);
        }
    }
};

int main () {
    
    cout << "Starting the game!!!" << endl;
    InitWindow(cellSize*cellCount, cellSize*cellCount, "Retro Snake");
    SetTargetFPS(60);

    Game game= Game();

    while(WindowShouldClose() == false){ //loop runs and updates 60 times/sec (FPS)
        BeginDrawing();

        if(eventTriggered(0.2)){
            game.Update();
        }

        if(IsKeyPressed(KEY_UP) && game.snake.direction.y != 1){
            game.snake.direction = {0, -1};
        }
        if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1){
            game.snake.direction = {0, 1};
        }
        if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1){
            game.snake.direction = {-1, 0};
        }
        if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1){
            game.snake.direction = {1, 0};
        }
        

        // Drawing
        ClearBackground(blue); //When called, previous game's graphics are wiped clean and new frame is drawn
        game.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}