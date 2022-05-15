#include <simplecpp>
#include "shooter.h"
#include "bubble.h"

/* Simulation Vars */
const double STEP_TIME = 0.02;

/* Game Vars */
const int PLAY_Y_HEIGHT = 450;
const int LEFT_MARGIN = 70;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);


void move_bullets(vector<Bullet> &bullets){
    // move all bullets
    for(unsigned int i=0; i<bullets.size(); i++){
        if(!bullets[i].nextStep(STEP_TIME)){
            bullets.erase(bullets.begin()+i);
        }
    }
}

void move_bubbles(vector<Bubble> &bubbles){
    // move all bubbles
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
    }
}


vector<Bubble> create_bubbles()
{
    // create initial bubbles in the game
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, (-1.5)*BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, BUBBLE_DEFAULT_AY, COLOR(150,0,150)));
    bubbles.push_back(Bubble(WINDOW_X/4.0, (1.5)*BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, (2.5)*BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, BUBBLE_DEFAULT_AY, COLOR(150,0,150)));
    bubbles.push_back(Bubble(WINDOW_X/3.0, 2*BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, (-2)*BUBBLE_DEFAULT_VX, BUBBLE_DEFAULT_VY, BUBBLE_DEFAULT_AY, COLOR(150,0,150)));
    return bubbles;
}

int main()
{
    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);

    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));

    string msg_cmd("Cmd: _");
    Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);

    string msg_score("Score: ");
    Text charScored(WINDOW_X - LEFT_MARGIN, BOTTOM_MARGIN, msg_score);

    string msg_life("Health Remaining:");
    Text charLife(WINDOW_X - LEFT_MARGIN, TOP_MARGIN, msg_life);

    string msg_time("Time left: ");
    Text charTime(LEFT_MARGIN, TOP_MARGIN, msg_time);

    // Intialize the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

    // Initialize the bubbles
    vector<Bubble> bubbles = create_bubbles();

    // Initialize the bullets (empty)
    vector<Bullet> bullets;
    double timer = 30.00; // Initiaize the timer, health counter and score variables
    int life = 3;
    int score = 0;

    bool check[bubbles.size()];                        // check is a proxy boolean variable array
    bool shooter_collision[bubbles.size()]; /* shooter_collision and proxy ensure life does not go to zero after only one touch of
                                           shooter with bubble because of shooter_collision being true every STEP_TIME interval */
    for(int i = 0; i < bubbles.size(); i++){
      check[i] = true;
      shooter_collision[i] = false;
    }             // array is created so that when two bubles colide with shooter simultaneously life decreases by 2

    XEvent event;

    // Main game loop
    while (true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }


     for(unsigned int i = 0; i < bullets.size(); i++){

        for(unsigned int j = 0; j < bubbles.size(); j++){

          if((pow((bullets[i].get_center_x() - (bullets[i].get_width())/2 - bubbles[j].get_center_x()), 2) + pow((bullets[i].get_center_y() - (bullets[i].get_height())/2 - bubbles[j].get_center_y()), 2) <= bubbles[j].get_radius()*bubbles[j].get_radius())
           ||
             (pow((bullets[i].get_center_x() + (bullets[i].get_width())/2 - bubbles[j].get_center_x()), 2) + pow((bullets[i].get_center_y() - (bullets[i].get_height())/2 - bubbles[j].get_center_y()), 2) <= bubbles[j].get_radius()*bubbles[j].get_radius())
           ||
             (pow((bullets[i].get_center_x() - (bullets[i].get_width())/2 - bubbles[j].get_center_x()), 2) + pow((bullets[i].get_center_y() + (bullets[i].get_height())/2 - bubbles[j].get_center_y()), 2) <= bubbles[j].get_radius()*bubbles[j].get_radius())
           ||
             (pow((bullets[i].get_center_x() + (bullets[i].get_width())/2 - bubbles[j].get_center_x()), 2) + pow((bullets[i].get_center_y() + (bullets[i].get_height())/2 - bubbles[j].get_center_y()), 2) <= bubbles[j].get_radius()*bubbles[j].get_radius())){

        // condition for collision: one of the corners of the square bullet should lie inside the circular bubble

             bullets.erase(bullets.begin()+i);
             if(bubbles[j].get_radius() <= BUBBLE_RADIUS_THRESHOLD){

                score += 100;
                bubbles.erase(bubbles.begin()+j);}

             else{

                score += 50;
                double X = bubbles[j].get_center_x();
                double Y = bubbles[j].get_center_y();
                bubbles.erase(bubbles.begin()+j);
                bubbles.push_back(Bubble(X, Y, BUBBLE_DEFAULT_RADIUS/2, -BUBBLE_DEFAULT_VX, 0, BUBBLE_DEFAULT_AY*2, COLOR(255,105,180)));
                bubbles.push_back(Bubble(X, Y, BUBBLE_DEFAULT_RADIUS/2, BUBBLE_DEFAULT_VX, 0, BUBBLE_DEFAULT_AY*2, COLOR(255,105,180)));}
// erasing original bubble and adding a two new ones having different colour from parent bubble

        }


       }
   }

      Text charScorecheck(WINDOW_X - LEFT_MARGIN + msg_score.length()*3 + 15, BOTTOM_MARGIN, score);
      charScorecheck.imprint();


      for(unsigned int j = 0; j < bubbles.size(); j++){

         if((pow((shooter.get_head_center_x() - bubbles[j].get_center_x()), 2) + pow((shooter.get_head_center_y() - bubbles[j].get_center_y()), 2)) <=
             pow(bubbles[j].get_radius() + shooter.get_head_radius(), 2)
            ||
            (pow((shooter.get_body_center_x() - (shooter.get_body_width())/2 - bubbles[j].get_center_x()), 2) + pow((shooter.get_body_center_y() - (shooter.get_body_height())/2 - bubbles[j].get_center_y()), 2) <= bubbles[j].get_radius()*bubbles[j].get_radius())
            ||
            (pow((shooter.get_body_center_x() + (shooter.get_body_width())/2 - bubbles[j].get_center_x()), 2) + pow((shooter.get_body_center_y() - (shooter.get_body_height())/2 - bubbles[j].get_center_y()), 2) <= bubbles[j].get_radius()*bubbles[j].get_radius())
            ||
            (pow((shooter.get_body_center_x() - (shooter.get_body_width())/2 - bubbles[j].get_center_x()), 2) + pow((shooter.get_body_center_y() + (shooter.get_body_height())/2 - bubbles[j].get_center_y()), 2) <= bubbles[j].get_radius()*bubbles[j].get_radius())
            ||
            (pow((shooter.get_body_center_x() + (shooter.get_body_width())/2 - bubbles[j].get_center_x()), 2) + pow((shooter.get_body_center_y() + (shooter.get_body_height())/2 - bubbles[j].get_center_y()), 2) <= bubbles[j].get_radius()*bubbles[j].get_radius())){

                shooter_collision[j] = true;

           }
         else shooter_collision[j] = false;

         if(shooter_collision[j] && check[j]){

            life -= 1;
            check[j] = false;
          }

         if(!shooter_collision[j] && !check[j]){
            check[j] = true;}


      }



     Text charHealth(WINDOW_X - LEFT_MARGIN + msg_life.length()*4 - 3, TOP_MARGIN, life);
     charHealth.imprint();


     timer -= STEP_TIME*2;  // by using int(timer), timer will be displayed as its floor function hence change every second

     if(timer >= 10){
     Text charTimer(LEFT_MARGIN + msg_time.length()*4, TOP_MARGIN, int(timer));
     charTimer.imprint();}

     else{
     stringstream ss;
     ss << int(timer);
     string str = ss.str();
     string str1 = "  ";
     Text charTimer(LEFT_MARGIN + msg_time.length()*4, TOP_MARGIN, str1.append(str));
     charTimer.imprint();
     }


     if(life <= 0){

        Text loser((WINDOW_X)/2, (WINDOW_Y)/2, "GAME OVER: NO HEALTH LEFT!");
        loser.setColor(COLOR(255,0,0));
        loser.scale(100);
        wait(5);
        break;
      }

     if(bubbles.size() == 0){

        Text winner((WINDOW_X)/2, (WINDOW_Y)/2, "CONGRATULATIONS! YOU WON!");
        winner.setColor(COLOR(0,255,0));
        winner.scale(100);
        wait(5);
        break;

      }

// Update the bubbles
        move_bubbles(bubbles);

        // Update the bullets
        move_bullets(bullets);

        wait(STEP_TIME);

        if(timer < 0){

              Text lost((WINDOW_X)/2, (WINDOW_Y)/2, "GAME OVER: TIME IS UP!");
              lost.setColor(COLOR(255,0,0));
              lost.scale(100);
               wait(5);
               break;
            }


    }
}
