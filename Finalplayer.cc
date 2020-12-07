
#include "Player.hh"
#include <complex>
#include <vector>
#include <map>
using namespace std;


/**
 * Write the name of your player and save this file
 * as AI<name>.cc
 */
#define PLAYER_NAME Demo

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

  vector<Dir> all_dirs;

  typedef pair<bool,Dir> columns;

  typedef vector<vector<columns>> Matrix;


  Dir backtracking(Matrix& z, const Pos& pfinal, Pos& pinicial){
    Dir d = SLOW;
    int nc = number_universe_columns();
    /*
    for(int i = 0; i < number_rows(); ++i){
      for(int j = 0; j < number_window_columns(); ++j){
        if(z[i][j%nc].second == SLOW_UP) cerr<<"SU ";
        if(z[i][j%nc].second == SLOW_DOWN) cerr<<"SD ";
        if(z[i][j%nc].second == UP) cerr<<"UP ";
        if(z[i][j%nc].second == DEFAULT) cerr<<"DF ";
        if(z[i][j%nc].second == DOWN) cerr<<"DO ";
        if(z[i][j%nc].second == FAST_UP) cerr<<"FU ";
        if(z[i][j%nc].second == FAST) cerr<<"F ";
        if(z[i][j%nc].second == FAST_DOWN) cerr<<"FD ";
        else cerr<<"SL ";
      }
      cerr<<endl;
    }*/
    while(pinicial != pfinal){
      Dir aux = z[first(pinicial)][second(pinicial)%nc].second;
      if(pfinal == pinicial - aux) d = aux;
      pinicial -= aux;
    }
    return d;
  }

  bool BFS(const Car& c){
    Matrix x(number_rows(),vector<columns>(number_universe_columns(),make_pair(false,SLOW)));
    queue<Pos> q;
    Pos p = c.pos;
    int i = first(p);
    int j = second(p);
    int nc = number_universe_columns();
    x[i][j%nc].first = true;
    q.push(p);
    while(not q.empty()){
      Pos act = q.front();
      q.pop();
      for(Dir d : all_dirs){
        i = first(act + d);
        j = second(act + d);
        if(within_window(act+d,round()+1) and x[i][j%nc].first == false){
          CType t = cell(act+d).type;
          if(t != MISSILE and t != TYRE and t != CAR){
            bool libre = true;
            if(d != DEFAULT and d != SLOW_UP  and d != SLOW_DOWN and libre){//no need to check path for SLOW_UP/SLOW_DOWN/DEFAULT
              if(d == UP){
                Dir pre = DEFAULT;
                CType r = cell(act+pre).type;
                if(r == MISSILE or r == TYRE or r == CAR){
                  libre = false;
                }
                if(libre){
                  pre = SLOW_UP;
                  r = cell(act+pre).type;
                  if(r == MISSILE or r == TYRE or r == CAR){
                    libre = false;
                  }
                }
              }
              else if(d == DOWN){
                Dir pre = DEFAULT;
                CType r = cell(act+pre).type;
                if(r == MISSILE or r == TYRE or r == CAR){
                  libre = false;
                }
                if(libre){
                  pre = SLOW_DOWN;
                  r = cell(act+pre).type;
                  if(r == MISSILE or r == TYRE or r == CAR){
                  libre = false;
                  }
                }
              }
              else if(d == FAST){
                Dir pre = DEFAULT;
                CType r = cell(act+pre).type;
                if(r == MISSILE or r == TYRE or r == CAR){
                libre = false;
                }
              }
              else if(d == FAST_UP){//def fast SLOW_up up
                Dir pre = DEFAULT;
                CType r = cell(act+pre).type;
                if(r == MISSILE or r == TYRE or r == CAR){
                  libre = false;
                }
                if(libre){
                  Dir pre = FAST;
                  CType r = cell(act+pre).type;
                  if(r == MISSILE or r == TYRE or r == CAR){
                    libre = false;
                  }
                }
                if(libre){
                  Dir pre = SLOW_UP;
                  CType r = cell(act+pre).type;
                  if(r == MISSILE or r == TYRE or r == CAR){
                    libre = false;
                  }
                }
                if(libre){
                  Dir pre = UP;
                  CType r = cell(act+pre).type;
                  if(r == MISSILE or r == TYRE or r == CAR){
                    libre = false;
                  }
                }
              }
              else if(d == FAST_DOWN){ //def fast SLOW_DOWN down
                Dir pre = DEFAULT;
                CType r = cell(act+pre).type;
                if(r == MISSILE or r == TYRE or r == CAR){
                  libre = false;
                }
                if(libre){
                  Dir pre = FAST;
                  CType r = cell(act+pre).type;
                  if(r == MISSILE or r == TYRE or r == CAR){
                    libre = false;
                  }
                }
                if(libre){
                  Dir pre = SLOW_DOWN;
                  CType r = cell(act+pre).type;
                  if(r == MISSILE or r == TYRE or r == CAR){
                    libre = false;
                  }
                }
                if(libre){
                  Dir pre = DOWN;
                  CType r = cell(act+pre).type;
                  if(r == MISSILE or r == TYRE or r == CAR){
                    libre = false;
                  }
                }
              }
            }
            if(libre){
              x[i][j%nc].first = true;
              x[i][j%nc].second = d;
              if(t == WATER_BONUS or t == MISSILE_BONUS or t == GAS_BONUS){
                /*if(t == WATER_BONUS) cerr<<"WB"<<endl;
                else if(t == MISSILE_BONUS) cerr<<"MB"<<endl;
                else if(t == GAS_BONUS) cerr<<"GB"<<endl;
                cerr<<i<<" "<<j<<" "<<c.pos<<" "<<act+d<<endl;
                cerr<<endl;
                for(int i = 0; i < number_rows(); ++i){
                  for(int j = 0; j < number_window_columns(); ++j){
                    complex<int> position(i,j);
                    if(cell(position).type == CAR or cell(position).type == TYRE or cell(position).type == MISSILE) cerr<<0<<" ";
                    else cerr<<"."<<" ";
                  }
                  cerr<<endl;
                }
                cerr<<endl;*/
                Pos fin = act + d;
                Dir d = backtracking(x,c.pos,fin);
                move(c.cid,d);
                return true;
              }
              else if(t == EMPTY){
                q.push(act+d);
              }
            }
          }
        }
      }
    }
    return false;
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
    for (Car_Id cid = begin(me()); cid != end(me()); ++cid) {
      if (car(cid).alive) { // if the car is alive
        Pos p = car(cid).pos;
        // has missiles and there is an enemy in the cells ahead, shoot
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
