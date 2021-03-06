/*6512345 zy12345 Joe Blogs*/

#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_video.h"
#include "SDL/SDL_events.h"
#include <string>
#include <math.h>
#include <stdlib.h>
#include<ctime>
#define random(a,b) (rand()%(b-a+1)+a)

/////////////---------part 1: Global Variables ----------------------------//////////////
int game_continue=1;
int zombiehome_alive=1;
int dragonzombie_alive=1;
int bear_alive=1;
int zombie_alive=1;
int stone_alive=1;
int jone_alive=1;
int disappear_spear=1;
int disappear_ice=1;
int disappear_fire=0;
int hostage_rescued=0;
int win=0;
int presskey=0;
int lighting_strike=0;
int x_off=0; //hostage address is a random
int y_off=0;

 Uint8 *keystates = SDL_GetKeyState( NULL );
//Screen attributes, the same size with background picture
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;
const int SCREEN_BPP = 32;

//The frame rate
const int FRAME_RATE = 20;

//jone snow's dimensions
const int JONE_WIDTH = 112;
const int JONE_HEIGHT = 107;

//the fire size(fire means fire sent by my dragon)
const int FIRE_WIDTH=21;
const int FIRE_HEIGHT=20;

//the ice size(ice means fire sent by enemy's dragonzombie)
const int ICE_WIDTH=25;
const int ICE_HEIGHT=30;

//the spear size(spear means the ice spear throwed by the zombie)
const int SPEAR_WIDTH=25;
const int SPEAR_HEIGHT=40;

//The dimensions of the background picture
const int BG_WIDTH = 1200;
const int BG_HEIGHT = 800;

const int DRAGONZOMBIE_WIDTH = 120;
const int DRAGONZOMBIE_HEIGHT = 92;

const int BEAR_WIDTH =80;
const int BEAR_HEIGHT =75;

const int ZOMBIE_WIDTH = 120;
const int ZOMBIE_HEIGHT = 85;

//The surfaces
SDL_Surface *jone = NULL;
SDL_Surface *background = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *zombiehome = NULL;
SDL_Surface *stone = NULL;
SDL_Surface *fire = NULL;
SDL_Surface *zombie = NULL;
SDL_Surface *dragonzombie = NULL;
SDL_Surface *bear = NULL;
SDL_Surface *hostage = NULL;
SDL_Surface *spear = NULL;
SDL_Surface *ice = NULL;
SDL_Surface *message =NULL;
SDL_Surface *message1 =NULL;
SDL_Surface *lighting = NULL;
SDL_Surface *winpic = NULL;

//The event structure
SDL_Event event;

TTF_Font *font = NULL;
SDL_Color textColor = { 0,0,0};

//The music that will be played
Mix_Music *music = NULL;

//The camera, camera is the scope u can see
SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };


///////////////--------- part2: Class definition --------------//////////////////
class Zombie
{   public:
    int x, y,xm,ym;   //(x,y) is the left upper point of this obj, (xm,ym) is the center of this obj 
	Zombie(){
		x=600;
		y=720;
		xm=0;
		ym= 0 ;
	}
	void showzombie();
};

class Dragonzombie
{  public:
	int x, y,xm,ym;
	int xVel,yVel;
	void showdragonzombie();

Dragonzombie(){
   x=50;
   y=100;
   xm=0;
   ym=0;
   xVel= 8;

}
void movedragonzombie();
};

class Bear
{  public:
   int x, y,xm,ym;
   int xVel,yVel;
	Bear(){
    x = 300;
    y = 200;
	xm =0;
	ym=0;
    xVel = 5;
    yVel = 5;  //moving speed
}
	void showbear();
	void movebearx();
	void movebeary();
	void movebearx1();
	void movebeary1();
};


class Hostage
{  
public:
	int x, y;
	void showhostage();  //before rescued
	void showhostage1(); //after rescue
};


class Stone
{
    int x, y;
public:
	void showstone();
};




//the zombiehome
class ZombieHome
{ //The X and Y offsets of the zombiehome
    int x, y;
public:
	void showzombiehome();
};


//define the fire
class Fire
{public:
int x,y,xm,ym;
int xVel,yVel;

Fire(){
    x = 1200;  //initial don't show fire
    y =  0;
    xVel = 0;
    yVel = 0;
}
void fire_handle_input();
void movefire();
void showfire();
};

//define the ice(ice means fire sent by enemy's dragonzombie)
class Ice
{   public:
    int x, y,xm,ym;
    int yVel;

//Initializes the variables
Ice(){
    x = 1100;
    y = 100;
	xm=0;
	ym=0;
    yVel =10;
}
    void moveice();
    void showice();
};


//spear is throwed by the Zombie thrower
class Spear
{   public:
    int x, y,xm,ym, x_init, y_init;
    int yVel;
    //Initializes the variables
Spear(){
    x_init = 620+ZOMBIE_WIDTH/2;
    y_init = 720+ZOMBIE_HEIGHT/2;
    x = x_init;
    y = y_init;
	xm=0;
	ym=0;
    yVel =-15;
}
    void movespear();
    void showspear();
};


//The main character: jone snow
class Jone
{
    public:
    //The X and Y offsets of jone snow
    int x, y,w,h,xm,ym;
    //The velocity of jone snow
    int xVel, yVel;
    //Initializes the variables
    Jone();
    //Handles with key pressing and adjusts the jone's velocity
    void handle_input();
    //Moves the jone
    void move();
    //Shows the jone on the screen
    void show();
    //sets the camera game_continue the jone
    void set_camera();
    //handle if jone rush into enimies' home
    void judge();
};



//The timer
class Timer
{
    private:
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;

    public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    int get_ticks();

    //Checks the status of the timer
    bool is_started();
    bool is_paused();
};



////////////////-------------------- part3: local functions ---------------------///////////////
SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );

        //If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 255, 255, 255 ) );
        }
    }

    //Return the optimized surface
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    if( TTF_Init()== -1)
    {
        return false;
    }

     if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "Game of The Throne", NULL );

    //If everything initialized fine
    return true;
}

bool load_files()
{
    fire=load_image("fire.png");

    //Load the images
    jone = load_image( "jone2.jpg" );
    background = load_image( "background0.png" );
	zombiehome = load_image("ZombieHome.jpg");
    stone = load_image("stone.jpg");
    zombie = load_image("zombie2.jpg");
    dragonzombie = load_image("dragon1.jpg");
	bear = load_image("bear0.png");
	hostage = load_image("hostage.jpg");
	spear = load_image("spear.png");
	ice = load_image("ice.png");
	font = TTF_OpenFont("outcome.ttf",90);
	lighting= load_image("lighting.jpg");
    winpic = load_image("winpic.jpg");

	  if( zombiehome == NULL||zombie == NULL || dragonzombie==NULL||bear==NULL||hostage==NULL||fire==NULL||stone == NULL||jone == NULL||background == NULL||font==NULL||ice==NULL||spear==NULL||lighting==NULL)
    {
        return false;
    }

     music = Mix_LoadMUS( "music.mp3" );
     if( music == NULL )
    {
        return false;
    }

    //If everything loaded fine
    return true;
}




void clean_up()
{
    //Free the surfaces
    SDL_FreeSurface( jone );
    SDL_FreeSurface( background );
	SDL_FreeSurface(zombiehome);
	SDL_FreeSurface( message );
	SDL_FreeSurface( message1 );
	SDL_FreeSurface( stone );
	SDL_FreeSurface( zombie );
	SDL_FreeSurface( dragonzombie );
	SDL_FreeSurface( screen );
	SDL_FreeSurface( fire );
	SDL_FreeSurface( bear );
	SDL_FreeSurface( hostage);
	SDL_FreeSurface( spear );
	SDL_FreeSurface( ice );

	 Mix_FreeMusic( music );

	TTF_CloseFont( font );
	Mix_CloseAudio();

	TTF_Quit();

    //Quit SDL
    SDL_Quit();
}


////////////////////--------- part4: class implemantation ------------/////////////
Jone::Jone()
{
    //Initialize the offsets
    x = 1000;
    y = 350;
    xm=0;
	ym=0;
    //Initialize the velocity
    xVel = 0;
    yVel = 0;
}


void Fire::fire_handle_input()
{ //If a key was pressed
    if( event.type == SDL_KEYDOWN)
    {
        //Adjust the velocity
        switch( event.key.keysym.sym )
        {
		    case SDLK_w: yVel = -FIRE_HEIGHT;xVel=0;presskey=1;disappear_fire=0;break;
            case SDLK_s: yVel = FIRE_HEIGHT;xVel=0; presskey=1;disappear_fire=0;break;
            case SDLK_a: xVel = -FIRE_WIDTH;yVel=0; presskey=1;disappear_fire=0;break;
            case SDLK_d: xVel = FIRE_WIDTH;yVel=0; presskey=1;disappear_fire=0;break;
        }
    }
}




void Fire::movefire()
{
    //Move the fire left or right
    x += xVel;

    //If the fire go too far to the leftward or rightward
    if( ( x < 0 ) || ( x + FIRE_WIDTH-camera.x > SCREEN_WIDTH ) )
    {
        //move back
        //x -= xVel;
    }

    //Move the fire up or down
    y += yVel;

    //If the fire go too far upward or downward
    if( ( y < 0 ) || ( y + FIRE_HEIGHT-camera.y > SCREEN_HEIGHT ) )
    {
        //move back
        //y -= yVel;
    }
}


//Bear can move up,down,right and left, hunt down jone snow
void Bear::movebearx()
{
    x +=xVel;
    if((x-camera.x<0)||(x+BEAR_WIDTH-camera.x>SCREEN_WIDTH))
    {
        x -=xVel;
    }
}

void Bear::movebearx1()
{
    x +=-xVel;
    if((x-camera.x<0)||(x+BEAR_WIDTH-camera.x>SCREEN_WIDTH))
    {
        x -=xVel;
    }
}




void Bear::movebeary()
{
    y +=yVel;
    if((y-camera.y<0)||(y+BEAR_HEIGHT-camera.y>SCREEN_WIDTH))
    {
        y -=yVel;
    }
}

void Bear::movebeary1()
{
    y +=-yVel;
    if((y-camera.y<0)||(y+BEAR_HEIGHT-camera.y>SCREEN_WIDTH))
    {
        y -=yVel;
    }
}


void Dragonzombie::movedragonzombie()
{
   x += xVel;
   if((x-camera.x<0)||(x + DRAGONZOMBIE_WIDTH-camera.x>SCREEN_WIDTH))
   {
       x -=xVel;
   }
}

void Ice::moveice()
{
	//Move the ice up
    y += yVel;

    //If the ice go too far upward or downward
    if( ( y-camera.y < 0 ) || ( y + ICE_HEIGHT-camera.y > SCREEN_HEIGHT ) )
    {
        //move back
        //y -= yVel;
    }
}

void Spear::movespear()
{
    //Move the spear up or down
    if(zombie_alive==1){y += yVel;}
}

void Dragonzombie::showdragonzombie()
{ apply_surface( x-camera.x, y-camera.y, dragonzombie, screen);
}
void Hostage::showhostage()
{ 
  apply_surface( x-camera.x,y-camera.y, hostage, screen );
}
void Hostage::showhostage1()
{
    apply_surface(1100-camera.x,630-camera.y,hostage,screen);
}
void Bear::showbear()
{ apply_surface( x-camera.x, y-camera.y, bear, screen );
}

void Stone::showstone()
{ apply_surface( 1009-camera.x, 635-camera.y, stone, screen );
}


void Spear::showspear()
{
    apply_surface(x-camera.x, y-camera.y, spear, screen);
}

void Ice::showice()
{
    apply_surface(x-camera.x, y-camera.y, ice, screen);
}


void Fire::showfire()
{
    apply_surface(x-camera.x,  y-camera.y, fire, screen);
}
void Zombie::showzombie()
{apply_surface( 640-camera.x, 705-camera.y, zombie, screen);}

void ZombieHome::showzombiehome()
{
    //Show the zombiehome
    apply_surface( 10-camera.x, 10-camera.y, zombiehome, screen);
}


void Jone::handle_input()
{
    //If a key was pressed
    if( event.type == SDL_KEYDOWN )
    {
        //Adjust the velocity
        switch( event.key.keysym.sym )
        {
            case SDLK_UP: yVel -= JONE_HEIGHT / 4; break;
            case SDLK_DOWN: yVel += JONE_HEIGHT / 4; break;
            case SDLK_LEFT: xVel -= JONE_WIDTH / 4; break;
            case SDLK_RIGHT: xVel += JONE_WIDTH / 4; break;
        }
    }
    //If a key was released
    else if( event.type == SDL_KEYUP )
    {
        //Adjust the velocity
        switch( event.key.keysym.sym )
        {
            case SDLK_UP: yVel += JONE_HEIGHT / 4; break;
            case SDLK_DOWN: yVel -= JONE_HEIGHT / 4; break;
            case SDLK_LEFT: xVel += JONE_WIDTH / 4; break;
            case SDLK_RIGHT: xVel -= JONE_WIDTH / 4; break;
        }
    }
}

void Jone::move()
{
    //Move jone left or right
    x += xVel;

    //If jone go too far to the left or right
    if( ( x < 0 ) || ( x + JONE_WIDTH > BG_WIDTH ) )
    {
        //move back
        x -= xVel;
    }

    //Move jone up or down
    y += yVel;

    //If jone go too far up or down
    if( ( y < 0 ) || ( y + JONE_HEIGHT > BG_HEIGHT ) )
    {
        //move back
        y -= yVel;
    }
}

void Jone::show()
{
    //Show jone
    apply_surface( x - camera.x, y - camera.y, jone, screen );
}


//the camera's center is jone snow
void Jone::set_camera()
{
    camera.x = ( x + JONE_WIDTH / 2 ) - SCREEN_WIDTH / 2;
    camera.y = ( y + JONE_HEIGHT / 2 ) - SCREEN_HEIGHT / 2;

    //Keep the camera in boundary.
    if( camera.x < 0 )
    {
        camera.x = 0;
    }
    if( camera.y < 0 )
    {
        camera.y = 0;
    }
    if( camera.x > BG_WIDTH - camera.w )
    {
        camera.x = BG_WIDTH - camera.w;
    }
    if( camera.y > BG_HEIGHT - camera.h )
    {
        camera.y = BG_HEIGHT - camera.h;
    }
}



Timer::Timer()
{
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start()
{
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}

void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause()
{
    //If the timer is paused
    if( paused == true )
    {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    //If the timer is running
    if( started == true )
    {
        //If the timer is paused
        if( paused == true )
        {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
    return 0;
}



bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}

//jone runsh into zombies' home
void Jone::judge()
{
  if(x<210&&y<122&&zombiehome_alive==1)
  {game_continue=0;
  jone_alive=0;}
}


//////////////-------------- part5: Main function --------------------////////////////
int main( int argc, char* args[] )
{
    //Quit flag
    bool quit = false;

    Jone myJone;
	ZombieHome myZombieHome;
	Fire myFire;
	Stone myStone;
	Zombie yourZombie;
	Dragonzombie yourDragonzombie;
	Bear yourBear;
	Hostage yourHostage;
	Spear yourSpear;
	Ice yourIce;

    //The frame rate regulator
    Timer fps;

    //Initialize
    if( init() == false )
    {
        return 1;
    }

    //Load the files
    if( load_files() == false )
    {
        return 1;
    }

	srand((unsigned)time(NULL));
	int fate=random(1,5);
    x_off = random(0,300);
    y_off = random(0,150);

//when you lose the game, the screen will display "GAME OVER"
   message = TTF_RenderText_Solid( font,"GAME OVER",textColor);
  if( message == NULL )
  {
    return 1;
  }

//when you win the game,the screen will display "YOU WIN"
  message1 = TTF_RenderText_Solid( font,"YOU WIN",textColor);
  if( message1 == NULL )
  {
    return 1;
  }

    //While the user hasn't quitted
    while( quit == false )
    {
        //Start the frame timer
        fps.start();
        if( Mix_PlayingMusic() == 0 ){
            //play music
            if( Mix_PlayMusic( music, -1 ) == -1 ) {
                return 1;
            }
        }

        //While there's events to handle
        while( SDL_PollEvent( &event ) ) {
            //Handle events for jone
            myJone.handle_input();
            myFire.fire_handle_input();
            //If the user has quitted out the window
            if( event.type == SDL_QUIT ) {
                //Quit the program
                quit = true;
            }
        }
		

//make the fire's coordinate same to jone snow's center everytime when the player fires
        if(presskey==1) {
		myFire.x=myJone.x+ JONE_WIDTH/2;
		myFire.y=myJone.y+ JONE_HEIGHT/2;
		presskey=0;
        }
        myFire.movefire();

        //Move jone snow
        myJone.move();

		//move the enemy's dragonzombie
        yourDragonzombie.movedragonzombie();

		//when jone snow flies in low sky, zombie will throw the spear upwards to kill jone snow
        if(myJone.y> 300 && zombie_alive==1) {
		disappear_spear=0;
        }
        if(disappear_spear==0) {
        yourSpear.movespear();
        }

        //Set the camera
        myJone.set_camera();

        //make the enemy's UFO can fly following jone snow
        if(yourBear.x-myJone.x<0)
        {
        yourBear.movebearx();
        }

         if(yourBear.x-myJone.x>0)
        {
        yourBear.movebearx1();
        }


        if(yourBear.y-myJone.y<0)
        {
            yourBear.movebeary();
        }

          if(yourBear.y-myJone.y>0)
        {
            yourBear.movebeary1();
        }

        //Show the background
        apply_surface( 0, 0, background, screen, &camera );

		//20% probility of get lighting strike!
		if(fate==1 && win==1){
			lighting_strike=1; game_continue=0; jone_alive=0;
		}  

        
        //when content the judging condition ,display you win or lose on the screen
        if((game_continue==0&&win==0) || (lighting_strike==1))
        {
             apply_surface(450,300,message,screen);
        }


	    //show our stone
		if(stone_alive==1) {myStone.showstone();}
		myJone.judge();

	//control display the picture of hostage, only when zombie's home is destroied then can see the hostage
	if(hostage_rescued==0 && zombiehome_alive==0)
		{
            yourHostage.x = 10+x_off;
            yourHostage.y = 630-y_off;
		    yourHostage.showhostage();
		}

  //  if(myFire.x<210&&myFire.y<122&&zombiehome_alive==1&&disappear_fire==0)
  //  {zombiehome_alive=0;disappear_fire=1;}

//control the picture of zombiehome display or disappear
        if(zombiehome_alive==1)
		{myZombieHome.showzombiehome();
		}
//when jone snow rescue the hostage
		if(zombiehome_alive==0&& abs(myJone.x- yourHostage.x)<50&& abs(myJone.y-yourHostage.y)<50)
		{
              hostage_rescued=1;
		}
//judge when the the ice hit jone snow
		yourIce.xm=yourIce.x+ICE_WIDTH/2;
		yourIce.ym=yourIce.y+ICE_HEIGHT/2;
		if(abs(myJone.xm-yourIce.xm)<(ICE_WIDTH+JONE_WIDTH)/2&&abs(myJone.ym-yourIce.ym)<(ICE_HEIGHT+JONE_HEIGHT)/2&&jone_alive==1&&disappear_ice==0)
		{
		    game_continue=0;
		    jone_alive=0;
		}


        //the following four lines check if jone and dragonzombie have a collision
		yourDragonzombie.xm=yourDragonzombie.x+DRAGONZOMBIE_WIDTH/2;
	    yourDragonzombie.ym=yourDragonzombie.y+DRAGONZOMBIE_HEIGHT/2;
        myJone.xm=myJone.x+JONE_WIDTH/2;
        myJone.ym=myJone.y+JONE_HEIGHT/2;
	    //printf("myJone.xm=%d,myJone.ym=%d\n",myJone.xm,myJone.ym); //this sentence using for output coordinate so we can know where is wrong.

	    //judge if jone snow rushes into enemy' dragonzombie
        if(abs(myJone.xm-yourDragonzombie.xm)<(DRAGONZOMBIE_WIDTH+JONE_WIDTH)/2&&abs(myJone.ym-yourDragonzombie.ym)<(DRAGONZOMBIE_HEIGHT+JONE_HEIGHT)/2&&jone_alive==1&&dragonzombie_alive==1)
		{game_continue=0;
		jone_alive=0;
		dragonzombie_alive=0;}

		//judge if zombie's spear hits jone snow
        yourSpear.xm=yourSpear.x+SPEAR_WIDTH/2;
		yourSpear.ym=yourSpear.y+SPEAR_HEIGHT/2;
		if(abs(myJone.xm-yourSpear.xm)<(JONE_WIDTH+SPEAR_WIDTH)/2&&abs(myJone.ym-yourSpear.ym)<(JONE_HEIGHT+SPEAR_HEIGHT)/2&&jone_alive==1)
		{game_continue=0;
		jone_alive=0;
		disappear_spear=1;}

       if(disappear_fire==0){myFire.showfire();}
       //myFire.showfire();

       //when jone snow rush into something, it will disappear
	    if(game_continue==0) {jone_alive=0;}
		if(jone_alive==1) {myJone.show();}

        if(myFire.x<210&&myFire.y<122&&zombiehome_alive==1&&disappear_fire==0)
        {zombiehome_alive=0;disappear_fire=1;}

		//check the collsion of fire and dragonzombie
        myFire.xm=myFire.x+FIRE_WIDTH/2;
		myFire.ym=myFire.y+FIRE_HEIGHT/2;
        //printf("myFire.xm=%d,myFire.ym=%d,yourDragonzombie.xm=%d,yourDragonzombie.ym=%d\n",myFire.xm,myFire.ym,yourDragonzombie.xm,yourDragonzombie.ym);
		if(abs(myFire.xm-yourDragonzombie.xm)<(DRAGONZOMBIE_WIDTH+FIRE_WIDTH)/2&&abs(myFire.ym-yourDragonzombie.ym)<(DRAGONZOMBIE_HEIGHT+FIRE_HEIGHT)/2&&dragonzombie_alive==1&&disappear_fire==0)
		{dragonzombie_alive=0;disappear_fire=1;}

       if(dragonzombie_alive==1) {yourDragonzombie.showdragonzombie();}

       //check the collsion of fire and bear
	   yourBear.xm=yourBear.x+BEAR_WIDTH/2;
	   yourBear.ym=yourBear.y+BEAR_HEIGHT/2;
       //printf("myFire.xm=%d,myFire.ym=%d,yourBear.xm=%d,yourBear.ym=%d\n",myFire.xm,myFire.ym,yourBear.xm,yourBear.ym);
	   if(abs(myFire.xm-yourBear.xm)<(FIRE_WIDTH+BEAR_WIDTH)/2&&abs(myFire.ym-yourBear.ym)<(FIRE_HEIGHT+BEAR_HEIGHT)/2&&bear_alive==1&&disappear_fire==0)
       {bear_alive=0; disappear_fire=1;}

       //judge when jone snow rushes into bears
       if(abs(myJone.xm-yourBear.xm)<(JONE_WIDTH+BEAR_WIDTH)/2&&abs(myJone.ym-yourBear.ym)<(JONE_HEIGHT+BEAR_HEIGHT)/2&&jone_alive==1&&bear_alive==1)
       {bear_alive=0;
       jone_alive=0;
       game_continue=0;
       }

       //judge whether display the picture of bear or not
	   if(bear_alive==1) {yourBear.showbear();}


       //check the collsion of zombie and fire
       yourZombie.xm=yourZombie.x+ZOMBIE_WIDTH/2;
	   yourZombie.ym=yourZombie.y+ZOMBIE_HEIGHT/2;
	   
       //check if fire kills zombie
       if((abs(myFire.xm-yourZombie.xm)<(FIRE_WIDTH+ZOMBIE_WIDTH)/2)&&(abs(myFire.ym-yourZombie.ym)<(FIRE_HEIGHT+ZOMBIE_HEIGHT)/2)&&(zombie_alive==1)&&disappear_fire==0)
       {zombie_alive=0;disappear_fire=1;}

	   //when jone snow rushes into zombie, jone die, zombie die, spear disappear.
	   if((abs(myJone.xm-yourZombie.xm)<(JONE_WIDTH+ZOMBIE_WIDTH)/2)&&(abs(myJone.ym-yourZombie.ym)<(ZOMBIE_HEIGHT+JONE_HEIGHT)/2)&&(zombie_alive==1)) {
            zombie_alive=0;
            game_continue=0;
            jone_alive=0;
            disappear_spear=1;
        }
	   if(zombie_alive==1) {yourZombie.showzombie();}

       //when the ice fire of enemy's dragonzombie hit the player's throne , the player lose the game
	   if(yourIce.y>600) { game_continue=0; disappear_ice=1;stone_alive=0; }

       //when content the judgement condition ,the player win the game
	   if(hostage_rescued==1&&myJone.xm>1040&&myJone.ym>650&&game_continue==1) {win=1;}

       // when the enemy's dragonzombie flies to the headover of the player's stone,it will throw the spear
       if((disappear_spear==0)&&(zombie_alive==1)) { yourSpear.showspear();}
       if( yourSpear.y<=0){ yourSpear.y= yourSpear.y_init;}

		if(yourDragonzombie.x>1000&&dragonzombie_alive==1){
        yourIce.moveice();
		yourIce.showice();
		disappear_ice=0;}

       if(win==1&&game_continue==1&&lighting_strike==0) {
            apply_surface(450,300,message1,screen);
            //yourHostage.showhostage1();
            apply_surface(1000-camera.x,620-camera.y,winpic,screen);
        }

		if(lighting_strike==1){
			apply_surface(990,570, lighting, screen);
		}

        //Update the screen
        if( SDL_Flip( screen ) == -1 ) {return 1;}

        //Cap the frame rate
        if( fps.get_ticks() < 1000 / FRAME_RATE ) {
            SDL_Delay( ( 1000 / FRAME_RATE ) - fps.get_ticks() );
        }
    } //end the while loop

	//system("pause");
    //Clean up
    clean_up();

    return 0;
}
