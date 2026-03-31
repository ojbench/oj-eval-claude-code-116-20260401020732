#ifndef SRC_HPP
#define SRC_HPP
#include <iostream>
#include <utility>
#include <deque>

struct Map;
enum class instruction{UP, DOWN, LEFT, RIGHT, NONE};
//the instruction of the snake

bool is_food(Map* map, int x, int y);
//return true if the cell at (x,y) is food

bool is_wall(Map* map, int x, int y);
//return true if the cell at (x,y) is wall

void eat_food(Map* map, int x, int y);
//remove the food at (x,y)

int get_height(Map* map);
//return the height of the map

int get_width(Map* map);
//return the width of the map


struct Snake{
  // store the necessary information of the snake
  // use any data structure you like
  // you can also add any necessary function
  //------------------------------------------
  // TODO
  std::deque<std::pair<int, int>> body; // Store snake body positions (head first)
  instruction current_direction;
  bool should_grow; // Flag to indicate if snake ate food and should grow

  void initialize(int x, int y, instruction ins){
    // (x,y) is the position of the head of the snake, ins is the initial orientation of the snake
    //------------------------------------------
    // TODO
    body.clear();
    body.push_back({x, y});
    current_direction = ins;
    should_grow = false;
  }

  int get_length(){
    //return the length of the snake
    //------------------------------------------
    //TODO
    return body.size();
  }

  bool move(Map* map, instruction ins){
    //the current map and the next instruction
    //return false if the snake is dead
    //return true if the snake is alive
    //------------------------------------------
    //TODO

    // Determine the actual direction to move
    instruction next_direction = current_direction;

    if (ins != instruction::NONE) {
      // Check for reverse direction (instant death)
      if ((current_direction == instruction::UP && ins == instruction::DOWN) ||
          (current_direction == instruction::DOWN && ins == instruction::UP) ||
          (current_direction == instruction::LEFT && ins == instruction::RIGHT) ||
          (current_direction == instruction::RIGHT && ins == instruction::LEFT)) {
        return false; // Die from reversing direction
      }
      next_direction = ins;
    }

    current_direction = next_direction;

    // Calculate new head position
    int head_x = body.front().first;
    int head_y = body.front().second;
    int new_x = head_x;
    int new_y = head_y;

    switch (current_direction) {
      case instruction::UP:
        new_x = head_x - 1;
        break;
      case instruction::DOWN:
        new_x = head_x + 1;
        break;
      case instruction::LEFT:
        new_y = head_y - 1;
        break;
      case instruction::RIGHT:
        new_y = head_y + 1;
        break;
      case instruction::NONE:
        break;
    }

    // Check if new position is out of bounds
    if (new_x < 0 || new_x >= get_height(map) ||
        new_y < 0 || new_y >= get_width(map)) {
      return false; // Die from hitting boundary
    }

    // Check if new position hits a wall
    if (is_wall(map, new_x, new_y)) {
      return false; // Die from hitting wall
    }

    // Check if new position hits snake's own body (except the tail which will move)
    for (size_t i = 0; i < body.size() - 1; i++) {
      if (body[i].first == new_x && body[i].second == new_y) {
        return false; // Die from hitting own body
      }
    }

    // Check if there's food at the new position
    bool ate_food = is_food(map, new_x, new_y);
    if (ate_food) {
      eat_food(map, new_x, new_y);
      should_grow = true;
    }

    // Add new head
    body.push_front({new_x, new_y});

    // Remove tail if not growing
    if (!should_grow) {
      body.pop_back();
    } else {
      should_grow = false;
    }

    return true; // Survived
  }

  std::pair<int, std::pair<int,int>*> get_snake(){
    //return the length of the snake and a pointer to the array of the position of the snake
    //you can store the head as the first element
    //------------------------------------------
    //TODO
    int len = body.size();
    std::pair<int, int>* arr = new std::pair<int, int>[len];
    for (int i = 0; i < len; i++) {
      arr[i] = body[i];
    }
    return {len, arr};
  }
};

const int MaxWidth = 20;
struct Map{
  // store the necessary information of the map
  bool wall[MaxWidth][MaxWidth];
  bool food[MaxWidth][MaxWidth];
  int width, height;

  int get_height(){
    //return the height of the map
    //TODO
    return height;
  }

  int get_width(){
    //return the width of the map
    //TODO
    return width;
  }

  bool is_food(int x, int y){
    //return true if the cell at (x,y) is food
    //TODO
    return food[x][y];
  }

  void eat_food(int x, int y){
    //eat the food at (x,y)
    //TODO
    food[x][y] = false;
  }

  bool is_wall(int x, int y){
    //return true if the cell at (x,y) is wall
    //TODO
    return wall[x][y];
  }

  // use any data structure you like
  // you can also add any necessary function
  //------------------------------------------
  //TODO

  void initialize(Snake *snake){
    char str[MaxWidth];
    int head_x = -1 , head_y = -1;
    instruction ins;
    //------------------------------------------
    //TODO

    // Read map dimensions
    std::cin >> height >> width;

    // Initialize map arrays
    for (int i = 0; i < MaxWidth; i++) {
      for (int j = 0; j < MaxWidth; j++) {
        wall[i][j] = false;
        food[i][j] = false;
      }
    }

    // Read map layout
    for (int i = 0; i < height; i++) {
      std::cin >> str;
      for (int j = 0; j < width; j++) {
        if (str[j] == '#') {
          wall[i][j] = true;
        } else if (str[j] == '*') {
          food[i][j] = true;
        } else if (str[j] == '@') {
          head_x = i;
          head_y = j;
        }
      }
    }

    // Read initial direction
    std::cin >> str;
    switch (str[0]) {
      case 'U':
        ins = instruction::UP;
        break;
      case 'D':
        ins = instruction::DOWN;
        break;
      case 'L':
        ins = instruction::LEFT;
        break;
      case 'R':
        ins = instruction::RIGHT;
        break;
      default:
        ins = instruction::NONE;
    }

    //------------------------------------------
    snake->initialize(head_x, head_y, ins);
  }

  void print(Snake *snake){
    auto snake_body = snake->get_snake();
    //------------------------------------------
    //TODO

    int len = snake_body.first;
    std::pair<int, int>* positions = snake_body.second;

    // Create a temporary map to mark snake positions
    bool snake_map[MaxWidth][MaxWidth];
    for (int i = 0; i < MaxWidth; i++) {
      for (int j = 0; j < MaxWidth; j++) {
        snake_map[i][j] = false;
      }
    }

    // Mark snake positions (head is first)
    for (int i = 0; i < len; i++) {
      snake_map[positions[i].first][positions[i].second] = true;
    }

    // Print the map
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        if (i == positions[0].first && j == positions[0].second) {
          std::cout << '@'; // Head
        } else if (snake_map[i][j]) {
          std::cout << 'o'; // Body
        } else if (wall[i][j]) {
          std::cout << '#'; // Wall
        } else if (food[i][j]) {
          std::cout << '*'; // Food
        } else {
          std::cout << '.'; // Empty
        }
      }
      std::cout << std::endl;
    }

    // Clean up dynamically allocated memory
    delete[] positions;
  }
};

struct Game{
  Map *map;
  Snake *snake;
  int score;
  int round;
  void initialize(){
    map = new Map();
    snake = new Snake();
    map->initialize(snake);
    score = 0;
    round = 0;
  }
  bool step()
  {
    char str[MaxWidth];
    std::cin >> str;
    instruction ins;
    switch(str[0]){
      case 'U':
        ins = instruction::UP;
        break;
      case 'D':
        ins = instruction::DOWN;
        break;
      case 'L':
        ins = instruction::LEFT;
        break;
      case 'R':
        ins = instruction::RIGHT;
        break;
      default:
        ins = instruction::NONE;
    }
    if(snake->move(map, ins)){
      score++;
    }else{
      return false;
    }
    return true;
  }
  void print(){
    std::cout<< "Round: " << round << std::endl;
    map->print(snake);
  }
  void play()
  {
    while(step()){
      round++;
      print();
    }
    score += snake->get_length() ;
    std::cout << "Game Over" << std::endl;
    std::cout << "Score: " << score << std::endl;
  }
};
#endif
