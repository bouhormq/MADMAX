
#include "Player.hh"
#include <complex>
#include <vector>
#include <map>
using namespace std;


/**
 * Write the name of your player and save this file
 * as AI<name>.cc
 */
#define PLAYER_NAME WAHEDZERO

struct PLAYER_NAME : public Player {


    /**
     * Factory: returns a new instance of this class.
     * Do not modify this function.
     */
    static Player* factory () {
        return new PLAYER_NAME;
    }


    /**
     * Attributes for your player can be defined here.
     */

  vector<Dir> all_dirs; //vector for all posible positions

  typedef pair<bool,Dir> columns;

  typedef vector<vector<columns>> Matrix; //Path_saver matrix

  bool BFS(const Car& car){
    Matrix Path_saver(number_rows(),vector<columns>(number_universe_columns())); //Initialize Path_saver matrix with false (not visited) and SLOW (dont move)
    for(int i = 0; i < number_rows(); ++i){
      for(int j = 0; j < number_universe_columns();++j){
        make_pair(false,SLOW);
      }
    }
    Pos position = car.pos;
    int i = first(position);
    int j = second(position);
    Path_saver[i][j%number_universe_columns()].first = true; //mark first position as visited
    queue<Pos> To_be_visited;
    To_be_visited.push(position);
    while(!To_be_visited.empty()){ //while there is a cell to visit
      Pos current_position = To_be_visited.front(); //grab new cell to examin
      To_be_visited.pop();
      for(int x = 0; x < 9; ++x){ //check for all posible directions
        Dir dir = all_dirs[x];
        i = first(current_position + dir);
        j = second(current_position + dir);
        if(within_window(current_position+dir,round()+1) and Path_saver[i][j%number_universe_columns()].first == false){ //if current cell within window
          CType cell_current_position = cell(current_position+dir).type;
          if(cell_current_position != MISSILE and cell_current_position != TYRE and cell_current_position != CAR){ //if current cell posible to travel to
            bool free = true;
            if(dir != DEFAULT and dir != SLOW_UP  and dir != SLOW_DOWN and free){//no need to check path for SLOW_UP/SLOW_DOWN/DEFAULT but we must do so for the rest
              if(dir == UP){//for UP we must check if DEFAULT and SLOW_UP are posible
                Dir condition = DEFAULT;
                CType cell_condition = cell(current_position+condition).type;
                if(cell_condition == MISSILE or cell_condition == TYRE or cell_condition == CAR){
                  free = false;
                }
                if(free){
                  condition = SLOW_UP;
                  cell_condition = cell(current_position+condition).type;
                  if(cell_condition == MISSILE or cell_condition == TYRE or cell_condition == CAR){
                    free = false;
                  }
                }
              }
              else if(dir == DOWN){//for DOWN we must check if DEFAULT and SLOW_DOWN are posible
                Dir condition = DEFAULT;
                CType cell_condition = cell(current_position+condition).type;
                if(cell_condition == MISSILE or cell_condition == TYRE or cell_condition == CAR){
                  free = false;
                }
                if(free){
                  condition = SLOW_DOWN;
                  cell_condition = cell(current_position+condition).type;
                  if(cell_condition == MISSILE or cell_condition == TYRE or cell_condition == CAR){
                  free = false;
                  }
                }
              }
              else if(dir == FAST){//for FAST we must check if DEFAULT are possible
                Dir condition = DEFAULT;
                CType cell_condition = cell(current_position+condition).type;
                if(cell_condition == MISSILE or cell_condition == TYRE or cell_condition == CAR){
                free = false;
                }
              }
              else if(dir == FAST_UP){//for FAST_UP we must check if DEFAULT,FAST,SLOW_UP and UP are possible
                Dir condition = DEFAULT;
                CType cell_condition = cell(current_position+condition).type;
                if(cell_condition == MISSILE or cell_condition == TYRE or cell_condition == CAR){
                  free = false;
                }
                if(free){
                  Dir condition = FAST;
                  CType cell_condition = cell(current_position+condition).type;
                  if(cell_condition == MISSILE or cell_condition == TYRE or cell_condition == CAR){
                    free = false;
                  }
                }
                if(free){
                  Dir condition = SLOW_UP;
                  CType cell_condition = cell(current_position+condition).type;
                  if(cell_condition == MISSILE or cell_condition == TYRE or cell_condition == CAR){
                    free = false;
                  }
                }
                if(free){
                  Dir condition = UP;
                  CType cell_condition = cell(current_position+condition).type;
                  if(cell_condition == MISSILE or cell_condition == TYRE or cell_condition == CAR){
                    free = false;
                  }
                }
              }
              else if(dir == FAST_DOWN){ //for FAST_DOWN we must check if DEFAULT,FAST,SLOW_DOWN and DOWN are possible
                Dir condition = DEFAULT;
                CType cell_condition = cell(current_position+condition).type;
                if(cell_condition == MISSILE or cell_condition == TYRE or cell_condition == CAR){
                  free = false;
                }
                if(free){
                  Dir condition = FAST;
                  CType cell_condition = cell(current_position+condition).type;
                  if(cell_condition == MISSILE or cell_condition == TYRE or cell_condition == CAR){
                    free = false;
                  }
                }
                if(free){
                  Dir condition = SLOW_DOWN;
                  CType cell_condition = cell(current_position+condition).type;
                  if(cell_condition == MISSILE or cell_condition == TYRE or cell_condition == CAR){
                    free = false;
                  }
                }
                if(free){
                  Dir condition = DOWN;
                  CType cell_condition = cell(current_position+condition).type;
                  if(cell_condition == MISSILE or cell_condition == TYRE or cell_condition == CAR){
                    free = false;
                  }
                }
              }
            }
            if(free){//If position can be reached
              Path_saver[i][j%number_universe_columns()].first = true; //mark cell as visited
              Path_saver[i][j%number_universe_columns()].second = dir; //save dir in Path_saver matrix
              if(cell_current_position == WATER_BONUS or cell_current_position == MISSILE_BONUS or cell_current_position == GAS_BONUS){//if BONUS REACHED
                Pos final_position = current_position + dir;
                Dir dir = backtracking(Path_saver,car.pos,final_position);//backtrack and return next move
                move(car.cid,dir);//move car to direction dir
                return true;
              }
              else if(cell_current_position == EMPTY){//else queue to visit
                To_be_visited.push(current_position+dir);
              }
            }
          }
        }
      }
    }
    return false;
  }

  Dir backtracking(Matrix& Path_saver, const Pos& final_position, Pos& position){//function that substracts dir to find the first move the car has to make and returns it
    Dir dir = SLOW;
    int nc = number_universe_columns();
    while(position != final_position){
      Dir disposable = Path_saver[first(position)][second(position)%number_universe_columns()].second;
      if(final_position == position - disposable) dir = disposable;
      position = position - disposable;
    }
    return dir;
  }





  /**
   * Play method.
   *
   * This method will be invoked once per each round.
   * You have to read the board here to place your actions
   * for this round.
   *
   */
  virtual void play () {
    if(round() == 0){
      // Initialize a vector with all allowed directions.
      all_dirs = {     UP, DEFAULT,      DOWN,
                  SLOW_UP,    SLOW, SLOW_DOWN,
                  FAST_UP,    FAST, FAST_DOWN};
    }
    // For each of my cars
    for (Car_Id cid = begin(me()); cid != end(me()); ++cid) {//go through all cars
      if (car(cid).alive) { // if the car is alive
        Pos p = car(cid).pos;
        // has missiles and there is an enemy ahead, shoot
        Cell ahead_close = cell(p + DEFAULT);
        Cell ahead_far = cell(p + FAST);
        if (car(cid).nb_miss > 0 and ahead_close.type == CAR) {
          Player_Id p = player_of(ahead_close.cid);
          if (p != me()) shoot(cid);
        }
        else if (car(cid).nb_miss > 0 and ahead_far.type == CAR) {
          Player_Id p = player_of(ahead_far.cid);
          if (p != me()) shoot(cid);
        }
        bool posible = BFS(car(cid));
      }
    }
  }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
