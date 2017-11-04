#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "project_v2.h"
#include "windows.h"
#include <cvintwrk.h>
#include "toolbox.h"
#include "bass.h"
static int panelHandle,panelHandle_2,panelHandle_3,panelHandle_4,panelHandle_5,panelHandle_6,panelHandle_7,panelHandle_8,panelHandle_9,panelHandle_10; 

#define speed_radix 10
#define limit_fire_screen_up 0
#define limit_fire_screen_down 800 
#define weaponmax 10  // max types of weapon and shots


//variables for keyboard monitoring
int Callback_Data;	
int Posting_Handle; 
int Keys_Down[256]={0}; 

HSTREAM weapon2_SND;
HSTREAM weapon3_SND;
HSTREAM weapon4_SND; 
HSTREAM weapon5_SND; 
HSTREAM music_SND;
HSTREAM music2_SND;
HSTREAM music3_SND;
HSTREAM select_weapon_SND;
HSTREAM enemy1_SND;
HSTREAM explosion1_SND; 
HSTREAM item_SND;  
HSTREAM smallmedkit1_SND; 
HSTREAM primery_completed_SND;
HSTREAM finish_SND; 
HSTREAM buttonclickrelease0005_SND;  
HSTREAM buttonrollover_SND;
HSTREAM seeker_SND;
HSTREAM primery_ahead_SND;


//Receives information from windows regarding key presses
int CVICALLBACK Key_DownFunc(int panel, int message, unsigned int* wParam, unsigned int* lParam, void* callbackData)
{
	unsigned int Key = *wParam;
	Keys_Down[Key] = 1;
	return 0;
}

//Receives information from windows regarding key releases
int CVICALLBACK Key_UpFunc(int panel, int message, unsigned int* wParam, unsigned int* lParam, void* callbackData)
{
	unsigned int Key = *wParam;
	Keys_Down[Key] = 0;
	return 0;
}

//asking windows to send keyboard press and release events to our software 
//Specifing 'Key_DownFunc' and 'Key_UpFunc' as the functions to call
void Connect_To_Windows_Events(void)
{
	InstallWinMsgCallback(panelHandle,WM_KEYDOWN,Key_DownFunc,VAL_MODE_IN_QUEUE,&Callback_Data,&Posting_Handle);
	InstallWinMsgCallback(panelHandle,WM_KEYUP,Key_UpFunc,VAL_MODE_IN_QUEUE,&Callback_Data,&Posting_Handle);
}

typedef struct
{
	double x, y,life,time,sizex,sizey,speed,doubleflag;
	int type,weapon;

} enemy_type;

typedef struct
{
	int x, y,id,damage,speed,speedx,clip,doubleflag;

} weapon_id_type;


typedef struct
{
	int x,y,type,flag,time;

} kill_effect_type;

typedef struct
{
	int index,flag;

} lock_missle_type;

typedef struct
{
	int type,speed,rate,speedf;
	double x,y;

} time_line_type;


typedef struct
{
	int prev,flag;


} location;

typedef struct
{
	int score,flag,full;
	char name[20];
	


} top_ten_type;






int x,y,c,rad,h,w,weapon,fy,fx,life,fire_shots_x[10000][10000],fire_shots_y[10000][10000],i,j,k,fire_rate_arr[100],shoot[5],flag,pweapon,time_index,enemy_counter,i_enemy,effect,effect_id[200],effect_time[1000],v2x,v2y,alpha,lock_index,shot_flag,enemy_flag,effect_flag,mssg_flag,flip[100],finnish_launch,flag_menu_sound[10],score,total_score,wplock[5],flag_music,time_music,boss,dir,map=1;   //project vals
double fire_rate[100],backx,counter,vol_effect,vol_music;
char name[20];
enemy_type enemy[1000];
kill_effect_type kill_effect[10000];
lock_missle_type lock_missile;
time_line_type timeline[60000];  //this store all the enemies and items in the game 
location prev_location[100];
top_ten_type top_ten[10];


//all the sprites
int radix_id;
int back1_id;
int NeutronC_id;
int BioMech10_id;
int Hud_id;
int RapidShield3_id;
int ExplosiveCannon3_id;
int PlasmaCannon3_id;
int PhaseTorpedoS3_id;
int SeekingMissileS3_id;
int BattleDrone2_id;
int BattleDrone8_id; 
int BlackBioMine1_id;
int BioSpawner3_id;
int pT5A7L8T9_id;
int thetruescout_id;
int Radix2Top2_id;
int Radix2Top3_id;
int bgd1_id;
int Seeker5_id;
int Missile5_id;
int boss3_id;



weapon_id_type weapon_struct[40];

int weapon_id[10];

void load_map()
{
	char line[100];
	FILE *fp;
	int cf,sp,tf,rf,spf=1;
	double xf,yf;
	cf=0;
	counter=0;
	SetCtrlVal(panelHandle, PANEL_BOSSLIFE,3000.00);
	SetCtrlAttribute (panelHandle, PANEL_BOSSLIFE, ATTR_VISIBLE, 0);
	score=0;
	boss=0;
	flag_music=0;// the left right sprites
	SetCtrlVal (panelHandle, PANEL_SCORE, score);
	effect=0;
	enemy_counter=0; //how much enemies made
	time_index=0;
	lock_missile.index=12;
	lock_missile.flag=1;
	finnish_launch=0;
	x=w/2;
	y=h-(45/2+1);
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS, radix_id, VAL_ENTIRE_OBJECT,MakeRect(x-25,y-25,x,y));
	for (i=0;i<60000;i++)
	{
		timeline[i].type=0;
		timeline[i].x=0;
		timeline[i].y=0;
		timeline[i].speed=0;
		timeline[i].rate=0;
		timeline[i].speedf=0;
	}
	i=1;
	j=0;
	
	switch (map){
		case 1:{
	fp=fopen("DATA\\DATA1.csv", "r");
		}
	break;
		case 2: {
	fp=fopen("DATA\\DATA2.csv", "r");
	BASS_ChannelStop(music_SND); 
	BASS_ChannelPlay(music2_SND,TRUE); 
		}
	break;
	
		case 3: {
	fp=fopen("DATA\\DATA3.csv", "r");
	BASS_ChannelStop(music2_SND); 
	BASS_ChannelPlay(music3_SND,TRUE); 

		}
	break;
	}
	
	while(fgets (line, 99, fp)!=NULL)
	{
		sscanf(line,"%d , %d , %lf , %lf , %d , %d, %d ", &cf ,&tf, &xf, &yf,&sp, &rf, &spf);			// index, type of enemy or item, x, y
		timeline[cf].type=tf;  //type
		timeline[cf].x=w/xf; //x location
		timeline[cf].y=yf; //y location
		timeline[cf].speed=sp;   //speed of enemy
		timeline[cf].rate=rf; //rate of fire
		timeline[cf].speedf=spf;// speed of fire
	}	   // while get the time line from the file
}// end load map

void insert_score()
{	
	int i_loc,min,i;
	i_loc=0;
	min=top_ten[0].score;

	for (i=1; i<10;i++)
	{
		if (min>top_ten[i].score)
		{
			min=top_ten[i].score;
			i_loc=i;
		
		}
		
		
	}   //for

	if (total_score>=min)
	{
		top_ten[i_loc].score=total_score;
		strcpy(top_ten[i_loc].name, name);
	
	}//if bigger then min

	
			
	
	
}

void music_player()				//this will make sure the music always on and replay itself
{
	switch (map)
	{
		case 1:					 	// play music map 1 
			if (flag_music==0)
			{
				flag_music=1;
				time_music=time_index+11700;
				BASS_ChannelPlay(music_SND,TRUE);
			}
			if (time_music<time_index)
			{
				flag_music=0;
				BASS_ChannelStop(music_SND);
			}
			break;
			
		case 2:					  	// play music map 2
			if (flag_music==0)
			{
				flag_music=1;
				time_music=time_index+11700;
				BASS_ChannelPlay(music2_SND,TRUE);
			}
			if (time_music<time_index)
			{
				flag_music=0;
				BASS_ChannelStop(music2_SND);
			}
			break;
			
		case 3:					// play music map 3
			if (flag_music==0)
			{
				flag_music=1;
				time_music=time_index+11700;
				BASS_ChannelPlay(music3_SND,TRUE);
			}
			if (time_music<time_index)
			{
				flag_music=0;
				BASS_ChannelStop(music3_SND);
			}
			break;
	}//end switch
}// end music player


void make_table()
{
	int i,c,d,swap;	
	char name[20];
	
	
 for (c = 0 ; c < ( 10 - 1 ); c++)		 /* Bubble sort code */
  	{
    for (d = 0 ; d < 10 - c - 1; d++)
    {
      if (top_ten[d].score < top_ten[d+1].score) /* For decreasing order use < */
      {
        swap = top_ten[d].score ;
		strcpy(name,top_ten[d].name);
		
        top_ten[d].score  = top_ten[d+1].score ;
		strcpy(top_ten[d].name,top_ten[d+1].name);
		
        top_ten[d+1].score  = swap;
		strcpy(top_ten[d+1].name, name);
      }
    }
  }
		for (i=1;i<11;i++)
		{
			SetTableCellVal (panelHandle_3, PANEL_3_TABLE, MakePoint(1,i+1), top_ten[i-1].name);
			SetTableCellVal (panelHandle_3, PANEL_3_TABLE, MakePoint(2,i+1),top_ten[i-1].score);	
		}
		
		
		FILE *fp; // declaring a FILE type pointer to handle the file

		fp=fopen("DATA\\top_ten.csv","w"); // open the file for writing

		for (i=0;i<10;i++)
		
		fprintf(fp,"%s ,%d \n",top_ten[i].name,top_ten[i].score); // write 10 lines of data
		
		fclose(fp); // close the file and save it
	
}									   

void start_new_game()
{
	music_SND = BASS_StreamCreateFile(FALSE,"Sound Effects\\music.ogg",0,0,0);
	music2_SND = BASS_StreamCreateFile(FALSE,"Sound Effects\\music2.ogg",0,0,0);
	music3_SND = BASS_StreamCreateFile(FALSE,"Sound Effects\\music3.ogg",0,0,0);
	wplock[1]=0;
	wplock[2]=0;
	wplock[3]=0;
	wplock[4]=0;
	SetCtrlAttribute (panelHandle, PANEL_TWO, ATTR_VISIBLE, 0);
	SetCtrlAttribute (panelHandle, PANEL_THREE, ATTR_VISIBLE, 0);
	SetCtrlAttribute (panelHandle, PANEL_FOUR, ATTR_VISIBLE, 0);
	SetCtrlAttribute (panelHandle, PANEL_FIVE, ATTR_VISIBLE, 0);
	BASS_ChannelSetAttribute(music_SND, BASS_ATTRIB_VOL, vol_music);
	BASS_ChannelSetAttribute(music2_SND, BASS_ATTRIB_VOL, vol_music); 
	BASS_ChannelSetAttribute(music3_SND, BASS_ATTRIB_VOL, vol_music);
	select_weapon_SND = BASS_StreamCreateFile(FALSE,"Sound Effects\\select_weapon.wav",0,0,0); 
	BASS_ChannelSetAttribute(select_weapon_SND, BASS_ATTRIB_VOL, vol_effect);
	
	enemy1_SND = BASS_StreamCreateFile(FALSE,"Sound Effects\\enemy1.wav",0,0,0); 
	BASS_ChannelSetAttribute(enemy1_SND, BASS_ATTRIB_VOL, vol_effect);
	explosion1_SND = BASS_StreamCreateFile(FALSE,"Sound Effects\\explosion1.wav",0,0,0); 
	BASS_ChannelSetAttribute(explosion1_SND, BASS_ATTRIB_VOL, vol_effect);
	item_SND = BASS_StreamCreateFile(FALSE,"Sound Effects\\item.wav",0,0,0); 
	BASS_ChannelSetAttribute(item_SND, BASS_ATTRIB_VOL, vol_effect);
	smallmedkit1_SND = BASS_StreamCreateFile(FALSE,"Sound Effects\\smallmedkit1.wav",0,0,0); 
	BASS_ChannelSetAttribute(smallmedkit1_SND, BASS_ATTRIB_VOL, vol_effect);
	primery_completed_SND = BASS_StreamCreateFile(FALSE,"Sound Effects\\primery_completed.wav",0,0,0); 
	BASS_ChannelSetAttribute(primery_completed_SND, BASS_ATTRIB_VOL, vol_effect);	  
	finish_SND = BASS_StreamCreateFile(FALSE,"Sound Effects\\finish.wav",0,0,0); 
	BASS_ChannelSetAttribute(finish_SND, BASS_ATTRIB_VOL, vol_effect);
	buttonclickrelease0005_SND = BASS_StreamCreateFile(FALSE,"Sound Effects\\buttonclickrelease0005.wav",0,0,0); 
	BASS_ChannelSetAttribute(buttonclickrelease0005_SND, BASS_ATTRIB_VOL, vol_effect);
	primery_ahead_SND = BASS_StreamCreateFile(FALSE,"Sound Effects\\primery_ahead.wav",0,0,0); 
	BASS_ChannelSetAttribute(primery_ahead_SND, BASS_ATTRIB_VOL, vol_effect);
	finnish_launch=0;

	map=1;																	
	life=100; //radix health
	total_score=0;
	SetCtrlVal (panelHandle, PANEL_HEATH, life);
	load_map(); //load the data from the map file
	mssg_flag=0; 
	weapon_struct[2].x=30;			  //define the weapon size and damage
	weapon_struct[2].y=30;			  //first weapon	  enegry
	weapon_struct[2].damage=10; 
	weapon_struct[2].speed=10;
	weapon_struct[2].speedx=0;
	weapon_struct[2].clip=-2; 
	weapon2_SND = BASS_StreamCreateFile(FALSE,"Sound Effects\\weapon2.wav",0,0,0);
	BASS_ChannelSetAttribute(weapon2_SND, BASS_ATTRIB_VOL, vol_effect);
	weapon_struct[3].x=35;		 // second weapon plasma
	weapon_struct[3].y=35;
	weapon_struct[3].damage=10; 
	weapon_struct[3].speed=10;
	weapon_struct[3].speedx=0;
	weapon_struct[3].clip=0;
	weapon3_SND = BASS_StreamCreateFile(FALSE,"Sound Effects\\weapon3.wav",0,0,0);
	BASS_ChannelSetAttribute(weapon3_SND, BASS_ATTRIB_VOL, vol_effect);
	SetCtrlVal (panelHandle, PANEL_W4,weapon_struct[3].clip );
	weapon_struct[4].x=50;			   //forth weapon lock missile
	weapon_struct[4].y=50;
	weapon_struct[4].damage=20; 
	weapon_struct[4].speed=5;
	weapon_struct[4].speedx=0;
	weapon_struct[4].clip=0;
	weapon4_SND = BASS_StreamCreateFile(FALSE,"Sound Effects\\weapon4.wav",0,0,0); 
	BASS_ChannelSetAttribute(weapon4_SND, BASS_ATTRIB_VOL, vol_effect);
	SetCtrlVal (panelHandle, PANEL_W3,weapon_struct[4].clip );
	weapon_struct[5].x=20;			// third weapon canon
	weapon_struct[5].y=20;
	weapon_struct[5].damage=8; 
	weapon_struct[5].speed=30;
	weapon_struct[5].speedx=0;
	weapon_struct[5].clip=0;
	weapon5_SND = BASS_StreamCreateFile(FALSE,"Sound Effects\\weapon5.wav",0,0,0); 
	BASS_ChannelSetAttribute(weapon5_SND, BASS_ATTRIB_VOL, vol_effect);
	SetCtrlVal (panelHandle, PANEL_W2,weapon_struct[5].clip );
	weapon_struct[1].x=20;			// fifthweapon canon
	weapon_struct[1].y=75;
	weapon_struct[1].damage=20; 
	weapon_struct[1].speed=10;
	weapon_struct[1].speedx=0;
	weapon_struct[1].clip=0;
	seeker_SND = BASS_StreamCreateFile(FALSE,"Sound Effects\\seeker.wav",0,0,0); 
	BASS_ChannelSetAttribute(seeker_SND, BASS_ATTRIB_VOL, vol_effect);
	SetCtrlVal (panelHandle, PANEL_W1,weapon_struct[1].clip );
				   // 5< is only enemy
	weapon_struct[6].x=40;
	weapon_struct[6].y=40;
	weapon_struct[7].x=40;
	weapon_struct[7].y=40;
	fire_rate_arr[1]=35;
	fire_rate_arr[2]=15;		 
	fire_rate_arr[3]=10;
	fire_rate_arr[4]=35;
	fire_rate_arr[5]=5;
	i=1;
	j=0;
//	flag=1;
	weapon=2;
	pweapon=weapon;
	shoot[2]=0;
	rad = 45; 
	srand (time(0));
	
	SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_WIDTH, rad);
	SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_COLOR, VAL_RED);
	CanvasClear (panelHandle, PANEL_CANVAS, VAL_ENTIRE_OBJECT);
	SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_WIDTH, 2);		   //rectable color and width
	SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_COLOR, MakeColor (255, 0, 0));
	CanvasStartBatchDraw (panelHandle, PANEL_CANVAS2);
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS2, Hud_id, VAL_ENTIRE_OBJECT, MakeRect (0, 0, 109, 1001));
	CanvasDrawBitmap (panelHandle, PANEL_WEAPONDISP,weapon_id[2], VAL_ENTIRE_OBJECT, MakeRect (0, 0, 49, 175));
} // end start new game





void setup()
{  
	FILE *fpt;
	char line[100];
	int i_loc;
	GetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_HEIGHT, &h);
	GetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_WIDTH, &w);
	
	vol_effect=1.0;
	vol_music=0.5;
	buttonclickrelease0005_SND = BASS_StreamCreateFile(FALSE,"Sound Effects\\buttonclickrelease0005.wav",0,0,0); 
	BASS_ChannelSetAttribute(buttonclickrelease0005_SND, BASS_ATTRIB_VOL, vol_effect);
	buttonrollover_SND = BASS_StreamCreateFile(FALSE,"Sound Effects\\buttonrollover.wav",0,0,0); 
	BASS_ChannelSetAttribute(buttonrollover_SND, BASS_ATTRIB_VOL, vol_effect);


	
	i_loc=0;
	fpt=fopen("DATA\\top_ten.csv", "r"); //read the top ten table
	
	
	while(fgets (line, 99, fpt)!=NULL)
	{
		sscanf(line,"%s , %d",top_ten[i_loc].name ,&top_ten[i_loc].score);			// index, type of enemy or item, x, y
	
		 
		i_loc++;
	}	   // while get the top ten from the f
 	
	make_table(); 
					  
	GetBitmapFromFile ("Radix2Top1.png", &radix_id);
	GetBitmapFromFile ("sky03.png", &back1_id); 
	GetBitmapFromFile ("NeutronC.png", &weapon_struct[2].id);
	GetBitmapFromFile ("Radix sprites\\SFX\\Energy1.png", &weapon_struct[3].id);
	GetBitmapFromFile ("Radix sprites\\Missiles\\Seeker5.png", &weapon_struct[1].id);
	GetBitmapFromFile ("Radix sprites\\SFX\\PhaseTorpedo1.png", &weapon_struct[4].id);
	GetBitmapFromFile ("Radix sprites\\SFX\\Burner2.png", &weapon_struct[5].id); 
	GetBitmapFromFile ("Radix sprites\\SFX\\AlienShot.png", &weapon_struct[6].id);
	GetBitmapFromFile ("Radix sprites\\SFX\\AlienShot.png", &weapon_struct[7].id);
	GetBitmapFromFile ("Radix sprites\\SFX\\AlienShot.png", &weapon_struct[8].id);
	GetBitmapFromFile ("Radix sprites\\SFX\\Burner1.png", &effect_id[0]);
	GetBitmapFromFile ("Radix sprites\\SFX\\Burner2.png", &effect_id[1]);
	GetBitmapFromFile ("Radix sprites\\SFX\\Burner3.png", &effect_id[2]);
	GetBitmapFromFile ("Radix sprites\\SFX\\ExplosionS1.png", &effect_id[3]);
	GetBitmapFromFile ("Radix sprites\\SFX\\ExplosionS2.png", &effect_id[4]);
	GetBitmapFromFile ("Radix sprites\\SFX\\ExplosionS3.png", &effect_id[5]);
	GetBitmapFromFile ("Radix sprites\\SFX\\ExplosionS4.png", &effect_id[6]);
	GetBitmapFromFile ("Radix sprites\\SFX\\ExplosionS5.png", &effect_id[7]);
	GetBitmapFromFile ("BioMech10.png", &BioMech10_id);  
	GetBitmapFromFile ("Radix sprites\\Enemies\\Battle Drone\\BattleDrone2.png", &BattleDrone2_id);
	GetBitmapFromFile ("Radix sprites\\Enemies\\Battle Drone\\BattleDrone8.png", &BattleDrone8_id);		 
	GetBitmapFromFile ("Radix sprites\\Enemies\\Black Bio-Mine\\BlackBioMine1.png", &BlackBioMine1_id);		  
	GetBitmapFromFile ("Radix sprites\\Enemies\\BioSpawner\\BioSpawner3.png", &BioSpawner3_id);		
	GetBitmapFromFile ("Radix sprites\\Enemies\\pT5A7L8T9.png", &pT5A7L8T9_id);
	GetBitmapFromFile ("Radix sprites\\Enemies\\thetruescout.png", &thetruescout_id);
	GetBitmapFromFile ("Radix sprites\\Enemies\\Radix\\Radix2Top2.png", &Radix2Top2_id);
	GetBitmapFromFile ("Radix sprites\\Enemies\\Radix\\Radix2Top3.png", &Radix2Top3_id);


	GetBitmapFromFile ("Hud.png", &Hud_id);
	GetBitmapFromFile ("Radix sprites\\Misc\\Hud\\Weapon1.png", &weapon_id[2]); // 1 blue energy 
	GetBitmapFromFile ("Radix sprites\\Misc\\Hud\\Weapon3.png", &weapon_id[3]); // 2 red energy 
	GetBitmapFromFile ("Radix sprites\\Misc\\Hud\\Weapon6.png", &weapon_id[4]); // 4 lock missile
	GetBitmapFromFile ("Radix sprites\\Misc\\Hud\\Weapon2.png", &weapon_id[5]); // chaingun
	GetBitmapFromFile ("Radix sprites\\Misc\\Hud\\Weapon4.png", &weapon_id[1]); // seeker 
	GetBitmapFromFile ("Radix sprites\\Items\\RapidShield3.png", &RapidShield3_id); // medic pack
	GetBitmapFromFile ("Radix sprites\\Items\\ExplosiveCannon3.png", &ExplosiveCannon3_id); //canon
	GetBitmapFromFile ("Radix sprites\\Items\\PlasmaCannon3.png", &PlasmaCannon3_id); //plasma 
	GetBitmapFromFile ("Radix sprites\\Items\\PhaseTorpedoS3.png", &PhaseTorpedoS3_id); // torpedo
	GetBitmapFromFile ("Radix sprites\\Items\\SeekingMissileS3.png", &SeekingMissileS3_id); // seeker 
	GetBitmapFromFile ("Radix sprites\\Enemies\\Snake Demon\\boss3.png", &boss3_id); // boss 

} //setup


void kill_boss()
{	 int i,r;
	for (i=0;i<75; i++)
			{
			r=rand() % 500;	
			timeline[time_index+r].type=250;
		}
		timeline[time_index+600].type=98;
	
}


void kill_radix()													 
{   int i,r; 
	for (i=0;i<50; i++)
			{
			r=rand() % 500;	
			timeline[time_index+r].type=150;
		}
	
		timeline[time_index+500].type=151;
			Keys_Down[38]=0;
			Keys_Down[40]=0;
			Keys_Down[37]=0;
			Keys_Down[39]=0;
			
	
}//end kill_radix

void speedradix()    //determaine the radix speed
{
	if (Keys_Down[38]==1)	 //up
	{   
		if (y>40)
		y=y-speed_radix;
	}// up
	if (Keys_Down[40]==1)//down
		if (y<h-10) 
		y=y+speed_radix;
	if (Keys_Down[37]==1)	 //left
		if (x>10) 
		{
			x=x-speed_radix;
		
			CanvasDrawBitmap (panelHandle, PANEL_CANVAS, Radix2Top2_id, VAL_ENTIRE_OBJECT,MakeRect(y-80,x-50,100,100));
		}

	if (Keys_Down[39]==1)	 //right
		if (x<w-10)
		{
		x=x+speed_radix;
	
		CanvasDrawBitmap (panelHandle, PANEL_CANVAS, Radix2Top3_id, VAL_ENTIRE_OBJECT,MakeRect(y-80,x-50,100,100));
		}
		
	

	

} //end speed radix	
	
void fire_rate_func()	// rate of fire
{
	
				fire_rate[weapon]--;
				
			
				
				
							if (fire_rate[weapon]<0  ) {	   
									fire_rate[weapon]=fire_rate_arr[weapon];// rate of fire  
									i++;
									if (weapon==2)
											BASS_ChannelPlay(weapon2_SND,TRUE); 
									if (weapon_struct[weapon].clip!=-2)
									{
										weapon_struct[weapon].clip--;
										switch (weapon) {
											case 3:			//plasma
												SetCtrlVal (panelHandle, PANEL_W4,weapon_struct[weapon].clip );
												BASS_ChannelPlay(weapon3_SND,TRUE);
												break;
											case 4:
												SetCtrlVal (panelHandle, PANEL_W3,weapon_struct[weapon].clip );
												BASS_ChannelPlay(weapon4_SND,TRUE);
										 
												break;
											case 5:
												SetCtrlVal (panelHandle, PANEL_W2,weapon_struct[weapon].clip );
												BASS_ChannelPlay(weapon5_SND,TRUE);
										 
												break;
											case 1:  //seeker
												SetCtrlVal (panelHandle, PANEL_W1,weapon_struct[weapon].clip );
												BASS_ChannelPlay(seeker_SND,TRUE);
										 
												break;
										}//switch
												
									} //if not -2, if its not the first weapon
									fire_shots_x[weapon][i]=x+3;
									fire_shots_y[weapon][i]=y-60;
									if (weapon==1 && weapon_struct[1].clip>0) 
									{
										fire_shots_x[weapon][i]=x+3-30;
										i++;
										weapon_struct[weapon].clip--;
										fire_shots_x[weapon][i]=x+3+30;
										fire_shots_y[weapon][i]=y-60;
									}
								
							
								
												} //if fire rate
							 				
				
								
							
	
								
} // end fire radix	


void locked_missile()
{   
	if (fire_shots_x[k][j]-(enemy[lock_missile.index].x) != 0)
	{
		if (fire_shots_x[k][j]>(enemy[lock_missile.index].x) && fire_shots_y[k][j]>(enemy[lock_missile.index].y)) 
		{
			alpha=((fire_shots_y[k][j]-(enemy[lock_missile.index].y)))/(fire_shots_x[k][j]-(enemy[lock_missile.index].x));
	
			if (alpha<=1 && alpha>=-1)
				{
					v2y=1*((asin(alpha)));
					fire_shots_y[k][j]=fire_shots_y[k][j]-v2y*(weapon_struct[2].speed+10);
					v2x=1*((acos(alpha)));
					fire_shots_x[k][j]=fire_shots_x[k][j]-v2x*(weapon_struct[2].speed+10);
				} // alpha
				
		
		} // plane 1
		
		if (fire_shots_x[k][j]>(enemy[lock_missile.index].x) && fire_shots_y[k][j]<(enemy[lock_missile.index].y)) 
		{
			alpha=(((enemy[lock_missile.index].y)-fire_shots_y[k][j]))/(fire_shots_x[k][j]-(enemy[lock_missile.index].x));
			if (alpha<=1 && alpha>=-1)
				{
					v2y=1*((asin(alpha)));
					fire_shots_y[k][j]=fire_shots_y[k][j]+(weapon_struct[2].speed+10);
					v2x=1*((acos(alpha)));
					fire_shots_x[k][j]=fire_shots_x[k][j]-(weapon_struct[2].speed+10);
				} // alpha
		} // plane 2 
		
		if (fire_shots_x[k][j]<(enemy[lock_missile.index].x) && fire_shots_y[k][j]>(enemy[lock_missile.index].y)) 
		{
			alpha=((fire_shots_y[k][j]-(enemy[lock_missile.index].y)))/((enemy[lock_missile.index].x)-fire_shots_x[k][j]);

			if (alpha<=1 && alpha>=-1)
				{
					v2y=1*((asin(alpha)));
					fire_shots_y[k][j]=fire_shots_y[k][j]-(weapon_struct[2].speed+10);
					v2x=1*((acos(alpha)));
					fire_shots_x[k][j]=fire_shots_x[k][j]+(weapon_struct[2].speed+10);
				} // alpha
		} // plane 3
		
		if (fire_shots_x[k][j]<(enemy[lock_missile.index].x) && fire_shots_y[k][j]<(enemy[lock_missile.index].y)) 
		{
			alpha=(((enemy[lock_missile.index].y)-fire_shots_y[k][j]))/((enemy[lock_missile.index].x)-fire_shots_x[k][j]);
		
			if (alpha<=1 && alpha>=-1)
				{
					v2y=1*((asin(alpha)));
					fire_shots_y[k][j]=fire_shots_y[k][j]+(weapon_struct[2].speed+10);
					v2x=1*((acos(alpha)));
					fire_shots_x[k][j]=fire_shots_x[k][j]+(weapon_struct[2].speed+10);
				} // alpha
		} // plane 4
	
	
		
		
		
	}// if not zero  .
	if ((enemy[lock_missile.index].life)<=0)	   //this remove the lock sign when dead
	{
		lock_missile.flag=0;
	}
		
	 
	
	
}//end locked missile

	
void movefire()  // move all the shows
{ 

	  for (k=1; k<weaponmax; k++)
	  {
		for (j=0; j<i+1; j++)
		{
			if (fire_shots_y[k][j]>limit_fire_screen_up && fire_shots_y[k][j]<limit_fire_screen_down ){ //stop the shots from going forver
		
				
			
			fire_shots_y[k][j]=fire_shots_y[k][j]-weapon_struct[k].speed; //speed of fire y
		
			
				if (weapon_struct[k].doubleflag==1)  //double shot  type 1
			{
			fire_shots_x[k][j]=fire_shots_x[k][j]-weapon_struct[k].speedx; //speed of fire x
		
				j++;
				fire_shots_y[k][j]=fire_shots_y[k][j]-weapon_struct[k].speed; //move left
				fire_shots_x[k][j]=fire_shots_x[k][j]+weapon_struct[k].speedx; 
			}//if double shot is on   type 1
			
			if (weapon_struct[k].doubleflag==2)  //double shot  type 2
			{
				j++;
				fire_shots_y[k][j]=fire_shots_y[k][j]-weapon_struct[k].speed; 
			}//if double shot is on   type 2
			if (k==4 && lock_missile.flag==1)  //this active the lock only for the weapon that can lock on target
			{
				locked_missile(); 
	
			}
		
		
		  
			} //if forever
			
		else 
		{
			fire_shots_y[k][j]=1000;
			fire_shots_x[k][j]=1000;
			
		}//else  .
	
			
				
		}//for   j
	  }//for k
		
	
} //move fire






void drawshots()   //draw all the shots
{
	
	  	  shot_flag=0; 
		  for (k=1; k<weaponmax; k++)
	  {
	for (j=0; j<i+1; j++){
		
	if (fire_shots_y[k][j]>limit_fire_screen_up && fire_shots_y[k][j]<limit_fire_screen_down ){ //stop the shots from going forver 
     		 CanvasDrawBitmap (panelHandle, PANEL_CANVAS,weapon_struct[k].id, VAL_ENTIRE_OBJECT,MakeRect(fire_shots_y[k][j],fire_shots_x[k][j]-17,weapon_struct[k].y,weapon_struct[k].x));     //fire
			 shot_flag=1; 
			} // end if
		 
			
		
	 
	} //end for j
	  }// end for k
 
	  
 } //end draw shots


void switchweapon () 
{
	if (Keys_Down['1']==1) 
	{
		pweapon=2;
		BASS_ChannelPlay(select_weapon_SND,TRUE);
		mssg_flag=1;
	}
	if (Keys_Down['2']==1 && wplock[2]==1) 
		{
		pweapon=3;
		BASS_ChannelPlay(select_weapon_SND,TRUE);
	}
	if (Keys_Down['5']==1 && wplock[4]==1) //torpedo
	{
		pweapon=4;
		BASS_ChannelPlay(select_weapon_SND,TRUE);
	
	}
	if (Keys_Down['3']==1 && wplock[3]==1) 
	{
		pweapon=5;
		BASS_ChannelPlay(select_weapon_SND,TRUE);
	
	}
	if (Keys_Down['4']==1 && wplock[1]==1) 		 //seeker
	{
		pweapon=1;
		BASS_ChannelPlay(select_weapon_SND,TRUE);
	
	}
	CanvasDrawBitmap (panelHandle, PANEL_WEAPONDISP, weapon_id[pweapon], VAL_ENTIRE_OBJECT, MakeRect (0, 0, 49, 175));
	
}	     

void collision()
{
	for (i_enemy=10; i_enemy<enemy_counter+10;i_enemy++)
		{ 
			if (enemy[i_enemy].life>0)
				{
						if  ((abs(y-enemy[i_enemy].y)<50)   && (abs(x-enemy[i_enemy].x)<(50)))	  //if radix touch somthing
						
							if ( enemy[i_enemy].type != -1 && enemy[i_enemy].type != -2 && enemy[i_enemy].type != -3 && enemy[i_enemy].type != -4 && enemy[i_enemy].type != -5)
							{
							kill_effect[effect].type=3;
							kill_effect[effect].x=enemy[i_enemy].x;
							kill_effect[effect].y=enemy[i_enemy].y;
							kill_effect[effect].time=time_index;
							effect++;
							enemy[i_enemy].y=-1000;
		    				enemy[i_enemy].x=-1000;
							enemy[i_enemy].life=-2;
							life=life-10;
							if (enemy[i_enemy].type==200)
								life=0;
							if (life<=0)   //if radix is dead
							{
								SetCtrlVal (panelHandle, PANEL_HEATH, 0);
								kill_radix();
							}//if radix is dead
							SetCtrlVal (panelHandle, PANEL_HEATH, life);
							if (i_enemy == lock_missile.index)
								lock_missile.flag=0;
							}//end if -1
					
						else 
						{
							
							switch (enemy[i_enemy].type)
							{
								case -1 ://medic	
									{
										score=score+10;
										SetCtrlVal (panelHandle, PANEL_SCORE, score);
										enemy[i_enemy].y=-1000;
		    							enemy[i_enemy].x=-1000;
										enemy[i_enemy].life=-2; 
										life=life+30;
										if (life>100)
											life=100; 
										SetCtrlVal (panelHandle, PANEL_HEATH, life); 
										BASS_ChannelPlay(smallmedkit1_SND,TRUE);
									}//end case 1
									break;
									
								case -2 ://canon	
									{
										pweapon=5; 
										wplock[3]=1;
										SetCtrlAttribute (panelHandle, PANEL_THREE, ATTR_VISIBLE, 1);
										score=score+10;
										SetCtrlVal (panelHandle, PANEL_SCORE, score);
										enemy[i_enemy].y=-1000;
		    							enemy[i_enemy].x=-1000;
										enemy[i_enemy].life=-2;
										
										weapon_struct[5].clip=weapon_struct[5].clip+500;
										if (weapon_struct[5].clip>1000)
											weapon_struct[5].clip=1000; 
										SetCtrlVal (panelHandle, PANEL_W2, weapon_struct[5].clip);
										BASS_ChannelPlay(item_SND,TRUE); 
									}//end case -2
									break;
									
									
								case -3 ://plasma	
									{
										pweapon=3; 
										wplock[2]=1;
										SetCtrlAttribute (panelHandle, PANEL_TWO, ATTR_VISIBLE, 1);
										score=score+10;
										SetCtrlVal (panelHandle, PANEL_SCORE, score);
										enemy[i_enemy].y=-1000;
		    							enemy[i_enemy].x=-1000;
										enemy[i_enemy].life=-2; 
										weapon_struct[3].clip=weapon_struct[3].clip+200;
										if (weapon_struct[3].clip>1000)
											weapon_struct[3].clip=1000; 
										SetCtrlVal (panelHandle, PANEL_W4, weapon_struct[3].clip); 
										BASS_ChannelPlay(item_SND,TRUE); 
									}//end case -3
									break;
									
								case -4 ://torpedo	
									{
										pweapon=4; 
										wplock[4]=1;
										SetCtrlAttribute (panelHandle, PANEL_FIVE, ATTR_VISIBLE, 1);
										score=score+10;
										SetCtrlVal (panelHandle, PANEL_SCORE, score);
										enemy[i_enemy].y=-1000;
		    							enemy[i_enemy].x=-1000;
										enemy[i_enemy].life=-2; 
										weapon_struct[4].clip=weapon_struct[4].clip+50;
										if (weapon_struct[4].clip>50)
											weapon_struct[4].clip=50; 
										SetCtrlVal (panelHandle, PANEL_W3, weapon_struct[4].clip); 
										BASS_ChannelPlay(item_SND,TRUE); 
									}//end case -4
									break;
									
								case -5 ://seeker	
									{
										pweapon=1;
										wplock[1]=1;
										SetCtrlAttribute (panelHandle, PANEL_FOUR, ATTR_VISIBLE, 1);
										score=score+10;
										SetCtrlVal (panelHandle, PANEL_SCORE, score);
										enemy[i_enemy].y=-1000;
		    							enemy[i_enemy].x=-1000;
										enemy[i_enemy].life=-2; 
										weapon_struct[1].clip=weapon_struct[1].clip+100;
										if (weapon_struct[1].clip>300)
											weapon_struct[1].clip=300; 
										SetCtrlVal (panelHandle, PANEL_W1, weapon_struct[1].clip); 
										BASS_ChannelPlay(item_SND,TRUE); 
									}//end case -5
									break;
							
							
							
								}//end switch	
						}//end else
						
					for (j=0; j<i+1; j++)
						{
							for  (k=1; k<6; k++)  
							{
						  
								if  ((abs(fire_shots_y[k][j]-enemy[i_enemy].y)<(enemy[i_enemy].sizey/2))   && (abs(fire_shots_x[k][j]-enemy[i_enemy].x)<(enemy[i_enemy].sizex/2))) 		  //collition to enemy
								{
									
									kill_effect[effect].type=1;
									kill_effect[effect].x=fire_shots_x[k][j];
									kill_effect[effect].y=fire_shots_y[k][j];
									kill_effect[effect].time=time_index;
									enemy[i_enemy].life=enemy[i_enemy].life-weapon_struct[k].damage;
									if (boss==1)
									{
										SetCtrlVal(panelHandle, PANEL_BOSSLIFE, enemy[i_enemy].life);
										if (enemy[i_enemy].life<=0)
										{
											kill_boss();
											score=score+1000;}}	
;
									effect++;
									if (enemy[i_enemy].life<=0)
									{
										score=score+100;
										SetCtrlVal (panelHandle, PANEL_SCORE, score);
										kill_effect[effect].type=3;
										kill_effect[effect].x=fire_shots_x[k][j];
										kill_effect[effect].y=fire_shots_y[k][j];
										kill_effect[effect].time=time_index;
										effect++;
									}//if its die
									fire_shots_y[k][j]=1000; //dont show the hits
									fire_shots_x[k][j]=1000;
							}//if  abs
							}//k
							
							for  (k=6; k<11; k++)   //collition to radix
							{
								if  ((abs(fire_shots_y[k][j]-y)<(25))   && (abs(fire_shots_x[k][j]-x)<(50))) 
								{
									life=life-weapon_struct[k].damage; //radix health
									if (life<=0)   //if radix is dead
									{
										SetCtrlVal (panelHandle, PANEL_HEATH, 0);
										kill_radix();
									}//if radix is dead
									SetCtrlVal (panelHandle, PANEL_HEATH, life);
									kill_effect[effect].type=2;
									
									kill_effect[effect].x=fire_shots_x[k][j];
									kill_effect[effect].y=fire_shots_y[k][j];
									fire_shots_y[k][j]=1000; //dont show the hits
									fire_shots_x[k][j]=1000;
									//kill_effect[effect].flag=1;
									kill_effect[effect].time=time_index;
									effect++;
								}// if abs radix
							} //for k radix
						 	   
						}//for  j	    y-80,x-50,100,100
						
					
				} // if life
			
				
		}// for i_enemy
}// end collision


void drawenemy()
{
	for (i_enemy=10; i_enemy<enemy_counter+10;i_enemy++)
		{
		
			if (enemy[i_enemy].life>0)
				{
					switch (enemy[i_enemy].type)
						{
							case 1 :
								{
									enemy[i_enemy].y=enemy[i_enemy].y+enemy[i_enemy].speed;
									if ((enemy[i_enemy].y=enemy[i_enemy].y) > (h/3) && ((enemy[i_enemy].x) < (w/4) || (enemy[i_enemy].x) > (3*w/4)))   //make it move to middle
										{
											if  ((enemy[i_enemy].x) < (w/2.5))
											enemy[i_enemy].x=enemy[i_enemy].x+4;
											else  
												enemy[i_enemy].x=enemy[i_enemy].x-1; 
										}
									CanvasDrawBitmap (panelHandle, PANEL_CANVAS, BioMech10_id, VAL_ENTIRE_OBJECT,MakeRect(enemy[i_enemy].y-(enemy[i_enemy].sizey/2),enemy[i_enemy].x-(enemy[i_enemy].sizex/2),enemy[i_enemy].sizex,enemy[i_enemy].sizey)); 
									enemy_flag=1; 
								}//case 1
								break;
								
							case 2 :
								{   if (	prev_location[i_enemy].flag==0)
									enemy[i_enemy].x=enemy[i_enemy].x+enemy[i_enemy].speed; 
								else 
									enemy[i_enemy].x=enemy[i_enemy].x-enemy[i_enemy].speed;    
									enemy[i_enemy].y=enemy[i_enemy].y+enemy[i_enemy].speed;
									if ((abs(prev_location[i_enemy].prev-enemy[i_enemy].x)) > 300)
									{
									prev_location[i_enemy].prev=enemy[i_enemy].x;
									if  (	prev_location[i_enemy].flag==1)
											prev_location[i_enemy].flag=0;
									else 
											prev_location[i_enemy].flag=1;
								
									}
									CanvasDrawBitmap (panelHandle, PANEL_CANVAS, thetruescout_id, VAL_ENTIRE_OBJECT,MakeRect(enemy[i_enemy].y-(enemy[i_enemy].sizey/2),enemy[i_enemy].x-(enemy[i_enemy].sizex/2),enemy[i_enemy].sizex,enemy[i_enemy].sizey)); 
									enemy_flag=1;
								}//case 2
								break;
								
								case 3 :
								{   
									enemy[i_enemy].x=enemy[i_enemy].x+enemy[i_enemy].speed; 
									enemy[i_enemy].y=enemy[i_enemy].y+enemy[i_enemy].speed;
									CanvasDrawBitmap (panelHandle, PANEL_CANVAS, BattleDrone8_id, VAL_ENTIRE_OBJECT,MakeRect(enemy[i_enemy].y-(enemy[i_enemy].sizey/2),enemy[i_enemy].x-(enemy[i_enemy].sizex/2),enemy[i_enemy].sizex,enemy[i_enemy].sizey)); 
									enemy_flag=1;
								}//case 3
								break;
								
							case 4 :
								{   
									enemy[i_enemy].x=enemy[i_enemy].x-enemy[i_enemy].speed; 
									enemy[i_enemy].y=enemy[i_enemy].y+enemy[i_enemy].speed;
									CanvasDrawBitmap (panelHandle, PANEL_CANVAS, BattleDrone2_id, VAL_ENTIRE_OBJECT,MakeRect(enemy[i_enemy].y-(enemy[i_enemy].sizey/2),enemy[i_enemy].x-(enemy[i_enemy].sizex/2),enemy[i_enemy].sizex,enemy[i_enemy].sizey)); 
									enemy_flag=1;
								}//case 4
								break;
								
							case 5 :
								{  
									enemy[i_enemy].y=enemy[i_enemy].y+enemy[i_enemy].speed;
									CanvasDrawBitmap (panelHandle, PANEL_CANVAS, pT5A7L8T9_id, VAL_ENTIRE_OBJECT,MakeRect(enemy[i_enemy].y-(enemy[i_enemy].sizey/2),enemy[i_enemy].x-(enemy[i_enemy].sizex/2),enemy[i_enemy].sizex,enemy[i_enemy].sizey)); 
									enemy_flag=1;
								}//case 5
								break; 
								
							case 6 :
								{  
									enemy[i_enemy].y=enemy[i_enemy].y+enemy[i_enemy].speed;
									CanvasDrawBitmap (panelHandle, PANEL_CANVAS, BioSpawner3_id, VAL_ENTIRE_OBJECT,MakeRect(enemy[i_enemy].y-(enemy[i_enemy].sizey/2),enemy[i_enemy].x-(enemy[i_enemy].sizex/2),enemy[i_enemy].sizex,enemy[i_enemy].sizey)); 
									enemy_flag=1;
								}//case 6
								break;
								
							case 7 :
								{  
									enemy[i_enemy].y=enemy[i_enemy].y+enemy[i_enemy].speed;
									CanvasDrawBitmap (panelHandle, PANEL_CANVAS, BlackBioMine1_id, VAL_ENTIRE_OBJECT,MakeRect(enemy[i_enemy].y-(enemy[i_enemy].sizey/2),enemy[i_enemy].x-(enemy[i_enemy].sizex/2),enemy[i_enemy].sizex,enemy[i_enemy].sizey)); 
									enemy_flag=1;
								}//case 7
								break;
								
								
							case -1 : // medic pack
								{
									enemy[i_enemy].y=enemy[i_enemy].y+enemy[i_enemy].speed;
									CanvasDrawBitmap (panelHandle, PANEL_CANVAS, RapidShield3_id, VAL_ENTIRE_OBJECT,MakeRect(enemy[i_enemy].y-(enemy[i_enemy].sizey/2),enemy[i_enemy].x-(enemy[i_enemy].sizex/2),enemy[i_enemy].sizex,enemy[i_enemy].sizey)); 
									enemy_flag=1;	
								}//case -1
								break;
								
							case -2 : // canon
								{
									enemy[i_enemy].y=enemy[i_enemy].y+enemy[i_enemy].speed;
									CanvasDrawBitmap (panelHandle, PANEL_CANVAS, ExplosiveCannon3_id, VAL_ENTIRE_OBJECT,MakeRect(enemy[i_enemy].y-(enemy[i_enemy].sizey/2),enemy[i_enemy].x-(enemy[i_enemy].sizex/2),enemy[i_enemy].sizex,enemy[i_enemy].sizey)); 
									enemy_flag=1;	
								}//case -2
								break;
								
							case -3 : // plasma
								{
									enemy[i_enemy].y=enemy[i_enemy].y+enemy[i_enemy].speed;
									CanvasDrawBitmap (panelHandle, PANEL_CANVAS, PlasmaCannon3_id, VAL_ENTIRE_OBJECT,MakeRect(enemy[i_enemy].y-(enemy[i_enemy].sizey/2),enemy[i_enemy].x-(enemy[i_enemy].sizex/2),enemy[i_enemy].sizex,enemy[i_enemy].sizey)); 
									enemy_flag=1;	
								}//case -3
								break;
								
							case -4 : // torpedo
								{
									enemy[i_enemy].y=enemy[i_enemy].y+enemy[i_enemy].speed;
									CanvasDrawBitmap (panelHandle, PANEL_CANVAS, PhaseTorpedoS3_id, VAL_ENTIRE_OBJECT,MakeRect(enemy[i_enemy].y-(enemy[i_enemy].sizey/2),enemy[i_enemy].x-(enemy[i_enemy].sizex/2),enemy[i_enemy].sizex,enemy[i_enemy].sizey)); 
									enemy_flag=1;	
								}//case -4
								break ;
							case -5 : // seeker
								{
									enemy[i_enemy].y=enemy[i_enemy].y+enemy[i_enemy].speed;
									CanvasDrawBitmap (panelHandle, PANEL_CANVAS, SeekingMissileS3_id, VAL_ENTIRE_OBJECT,MakeRect(enemy[i_enemy].y-(enemy[i_enemy].sizey/2),enemy[i_enemy].x-(enemy[i_enemy].sizex/2),enemy[i_enemy].sizex,enemy[i_enemy].sizey)); 
									enemy_flag=1;	
								}//case -5
								break ;
								
							case 200 : // boss
								{
									if (dir==0)
									enemy[i_enemy].y=enemy[i_enemy].y+enemy[i_enemy].speed;
									if (dir==1 && enemy[i_enemy].x<(w-100) && enemy[i_enemy].x>100)
										enemy[i_enemy].x=enemy[i_enemy].x+enemy[i_enemy].speed; 
									CanvasDrawBitmap (panelHandle, PANEL_CANVAS, boss3_id, VAL_ENTIRE_OBJECT,MakeRect(enemy[i_enemy].y-(enemy[i_enemy].sizey/2),enemy[i_enemy].x-(enemy[i_enemy].sizex/2),enemy[i_enemy].sizex,enemy[i_enemy].sizey)); 
									enemy_flag=1;	
								}//case 200
								break ;
										
								
						}  //switch
				}// if
		
		}//for
} // end draw enemy
	
			
void enemyfire()
{
	for (i_enemy=10; i_enemy<enemy_counter+10;i_enemy++)   
		{
	if (fire_rate_arr[i_enemy]!= -5){		

		fire_rate[i_enemy]--; 
		if (fire_rate[i_enemy]<0) 
			{
				if (enemy[i_enemy].life>0)
					{

						
					
						
						fire_rate[i_enemy]=fire_rate_arr[i_enemy];// rate of fire  
						i++;
						BASS_ChannelPlay(enemy1_SND,TRUE);
						fire_shots_x[enemy[i_enemy].weapon][i]=enemy[i_enemy].x;
						fire_shots_y[enemy[i_enemy].weapon][i]=enemy[i_enemy].y; 
						if (enemy[i_enemy].doubleflag==1)
							{
								i++;
								fire_shots_x[enemy[i_enemy].weapon][i]=enemy[i_enemy].x;
								fire_shots_y[enemy[i_enemy].weapon][i]=enemy[i_enemy].y;
							}// make the double shot  type 1
							
						if (enemy[i_enemy].doubleflag==2)
							{
								fire_shots_x[enemy[i_enemy].weapon][i]=fire_shots_x[enemy[i_enemy].weapon][i]-30;
								i++;
								fire_shots_x[enemy[i_enemy].weapon][i]=enemy[i_enemy].x+15;
								fire_shots_y[enemy[i_enemy].weapon][i]=enemy[i_enemy].y;
							}// make the double shot  type 2
				
					
							
					}// if life
			}	//id weapon
	}//if not -1
		}// for i enemy
		

}//enemy fire 

void draw_kill_enemy()
{
	for (i_enemy=10; i_enemy<enemy_counter+10;i_enemy++)
		{
			if (enemy[i_enemy].life>0 &&  enemy[i_enemy].y<limit_fire_screen_down)
				{
					 drawenemy();
				
				}	 // if alive
			else 
				{ //kill enemy
					enemy_flag=0;
					enemy[i_enemy].y=-1000;
		    		enemy[i_enemy].x=-1000;
					enemy[i_enemy].life=-2;
				
					if (i_enemy == lock_missile.index)
						lock_missile.flag=0;
				}// else kill
		}// for
}//end draw kill enemy


void time_line() {
	switch (timeline[time_index].type)
	{
		case 1 :		  
			{
				enemy[enemy_counter+10].x=timeline[time_index].x;
				enemy[enemy_counter+10].y=timeline[time_index].y;
				enemy[enemy_counter+10].sizex=75;
				enemy[enemy_counter+10].sizey=75; 
				enemy[enemy_counter+10].type=1;
				enemy[enemy_counter+10].life=20;
				enemy[enemy_counter+10].weapon=6; 
				enemy[enemy_counter+10].speed=timeline[time_index].speed;
				weapon_struct[enemy[enemy_counter+10].weapon].x=40;
				weapon_struct[enemy[enemy_counter+10].weapon].y=40;
				fire_rate_arr[enemy_counter+10]=timeline[time_index].rate;
				weapon_struct[enemy[enemy_counter+10].weapon].speed=-timeline[time_index].speedf;
				weapon_struct[enemy[enemy_counter+10].weapon].speedx=0;     //make the double shot
				weapon_struct[enemy[enemy_counter+10].weapon].damage=5;
				enemy[enemy_counter+10].doubleflag=0;	  //make the double shot
				weapon_struct[enemy[enemy_counter+10].weapon].doubleflag=0;    //make the double shot 
				enemy_counter++;

			}// case 1
				 break;
						  
		case 2 :		 
			{
			//	flag=1;
				enemy[enemy_counter+10].x=timeline[time_index].x;
				enemy[enemy_counter+10].y=timeline[time_index].y;
				enemy[enemy_counter+10].sizex=100;
				enemy[enemy_counter+10].sizey=100;
				enemy[enemy_counter+10].type=2;
				enemy[enemy_counter+10].life=20;
				enemy[enemy_counter+10].weapon=6;
				enemy[enemy_counter+10].speed=timeline[time_index].speed; 
				weapon_struct[enemy[enemy_counter+10].weapon].x=40;
				weapon_struct[enemy[enemy_counter+10].weapon].y=40;
				fire_rate_arr[enemy_counter+10]=timeline[time_index].rate; 
				weapon_struct[enemy[enemy_counter+10].weapon].speed=-timeline[time_index].speedf;
				weapon_struct[enemy[enemy_counter+10].weapon].speedx=0; 
				weapon_struct[enemy[enemy_counter+10].weapon].damage=5;
				enemy_counter++;

			}// case 2
				 break;	
				 
		case 3 :		 
			{

				enemy[enemy_counter+10].x=timeline[time_index].x;
				enemy[enemy_counter+10].y=timeline[time_index].y;
				enemy[enemy_counter+10].sizex=70;
				enemy[enemy_counter+10].sizey=120;
				enemy[enemy_counter+10].type=3;
				enemy[enemy_counter+10].life=20;
				enemy[enemy_counter+10].weapon=6;
				enemy[enemy_counter+10].speed=timeline[time_index].speed; 
				weapon_struct[enemy[enemy_counter+10].weapon].x=40;
				weapon_struct[enemy[enemy_counter+10].weapon].y=40;
				fire_rate_arr[enemy_counter+10]=timeline[time_index].rate; 
				weapon_struct[enemy[enemy_counter+10].weapon].speed=-timeline[time_index].speedf;
				weapon_struct[enemy[enemy_counter+10].weapon].speedx=0; 
				weapon_struct[enemy[enemy_counter+10].weapon].damage=5;
				enemy_counter++;
				 

			}// case 3
			
			 break;		
			
		case 4 :		 
			{

				enemy[enemy_counter+10].x=timeline[time_index].x;
				enemy[enemy_counter+10].y=timeline[time_index].y;
				enemy[enemy_counter+10].sizex=70;
				enemy[enemy_counter+10].sizey=120;
				enemy[enemy_counter+10].type=4;
				enemy[enemy_counter+10].life=20;
				enemy[enemy_counter+10].weapon=6;
				enemy[enemy_counter+10].speed=timeline[time_index].speed; 
				weapon_struct[enemy[enemy_counter+10].weapon].x=40;
				weapon_struct[enemy[enemy_counter+10].weapon].y=40;
				fire_rate_arr[enemy_counter+10]=timeline[time_index].rate; 
				weapon_struct[enemy[enemy_counter+10].weapon].speed=-timeline[time_index].speedf;
				weapon_struct[enemy[enemy_counter+10].weapon].speedx=0; 
				weapon_struct[enemy[enemy_counter+10].weapon].damage=5;
				enemy_counter++;
				 

			}// case 4
				 break;	
				 
				 
		case 5 :		 
			{

				enemy[enemy_counter+10].x=timeline[time_index].x;
				enemy[enemy_counter+10].y=timeline[time_index].y;
				enemy[enemy_counter+10].sizex=100;
				enemy[enemy_counter+10].sizey=100;
				enemy[enemy_counter+10].type=5;
				enemy[enemy_counter+10].life=40;
				enemy[enemy_counter+10].weapon=6;
				enemy[enemy_counter+10].speed=timeline[time_index].speed; 
				weapon_struct[enemy[enemy_counter+10].weapon].x=40;
				weapon_struct[enemy[enemy_counter+10].weapon].y=40;
				fire_rate_arr[enemy_counter+10]=timeline[time_index].rate; 
				weapon_struct[enemy[enemy_counter+10].weapon].speed=-2*timeline[time_index].speedf;
				weapon_struct[enemy[enemy_counter+10].weapon].speedx=timeline[time_index].speedf;     //make the double shot
				weapon_struct[enemy[enemy_counter+10].weapon].damage=5;
				enemy[enemy_counter+10].doubleflag=1;	  //make the double shot
				weapon_struct[enemy[enemy_counter+10].weapon].doubleflag=1;    //make the double shot 
				enemy_counter++;
				 

			}// case 5
			 break;
			
		case 6 :		 
			{

				enemy[enemy_counter+10].x=timeline[time_index].x;
				enemy[enemy_counter+10].y=timeline[time_index].y;
				enemy[enemy_counter+10].sizex=100;
				enemy[enemy_counter+10].sizey=100;
				enemy[enemy_counter+10].type=6;
				enemy[enemy_counter+10].life=40;
				enemy[enemy_counter+10].weapon=6;
				enemy[enemy_counter+10].speed=timeline[time_index].speed; 
				weapon_struct[enemy[enemy_counter+10].weapon].x=40;
				weapon_struct[enemy[enemy_counter+10].weapon].y=40;
				fire_rate_arr[enemy_counter+10]=timeline[time_index].rate; 
				weapon_struct[enemy[enemy_counter+10].weapon].speed=-timeline[time_index].speedf;
				weapon_struct[enemy[enemy_counter+10].weapon].speedx=0;     //make the double shot
				weapon_struct[enemy[enemy_counter+10].weapon].damage=5;
				enemy[enemy_counter+10].doubleflag=2;	  //make the double shot
				weapon_struct[enemy[enemy_counter+10].weapon].doubleflag=2;    //make the double shot 
				enemy_counter++;
				 

			}// case 6
				 break;	
				 
		case 7 :		 
			{

				enemy[enemy_counter+10].x=timeline[time_index].x;
				enemy[enemy_counter+10].y=timeline[time_index].y;
				enemy[enemy_counter+10].sizex=50;
				enemy[enemy_counter+10].sizey=50;
				enemy[enemy_counter+10].type=7;
				enemy[enemy_counter+10].life=20;
				enemy[enemy_counter+10].weapon=6;
				enemy[enemy_counter+10].speed=timeline[time_index].speed; 
				weapon_struct[enemy[enemy_counter+10].weapon].x=40;
				weapon_struct[enemy[enemy_counter+10].weapon].y=40;
				fire_rate_arr[enemy_counter+10]=-5;    //dont shoot
				weapon_struct[enemy[enemy_counter+10].weapon].speed=-timeline[time_index].speedf;
				weapon_struct[enemy[enemy_counter+10].weapon].speedx=0; 
				weapon_struct[enemy[enemy_counter+10].weapon].damage=5;
				enemy_counter++;
				 

			}// case 7
			 break;
				 
		case -1 :		   //medic pack 
			{
				enemy[enemy_counter+10].x=timeline[time_index].x;
				enemy[enemy_counter+10].y=timeline[time_index].y;
				enemy[enemy_counter+10].sizex=100;
				enemy[enemy_counter+10].sizey=150;
				enemy[enemy_counter+10].type=-1;
				enemy[enemy_counter+10].life=10000;
				enemy[enemy_counter+10].speed=timeline[time_index].speed; 
				fire_rate_arr[enemy_counter+10]=-5;   //dont shoot  
				enemy_counter++;
				
			}// case -1
				 break;	
				 
				 
				 
		case -2 :		   //canon
			{
				enemy[enemy_counter+10].x=timeline[time_index].x;
				enemy[enemy_counter+10].y=timeline[time_index].y;
				enemy[enemy_counter+10].sizex=100;
				enemy[enemy_counter+10].sizey=150;
				enemy[enemy_counter+10].type=-2;
				enemy[enemy_counter+10].life=10000;
				enemy[enemy_counter+10].speed=timeline[time_index].speed; 
				fire_rate_arr[enemy_counter+10]=-5;	     //dont shoot  
				enemy_counter++;
				 
			}// case -2
				 break;
				 
		 case -3 :		   //plasma
			{
				enemy[enemy_counter+10].x=timeline[time_index].x;
				enemy[enemy_counter+10].y=timeline[time_index].y;
				enemy[enemy_counter+10].sizex=100;
				enemy[enemy_counter+10].sizey=150;
				enemy[enemy_counter+10].type=-3;
				enemy[enemy_counter+10].life=10000;
				enemy[enemy_counter+10].speed=timeline[time_index].speed; 
				fire_rate_arr[enemy_counter+10]=-5;    //dont shoot  
				enemy_counter++;
			
			}// case -3
				 break;
				 
		  case -4 :		   //torpedo
			{
				enemy[enemy_counter+10].x=timeline[time_index].x;
				enemy[enemy_counter+10].y=timeline[time_index].y;
				enemy[enemy_counter+10].sizex=100;
				enemy[enemy_counter+10].sizey=150;
				enemy[enemy_counter+10].type=-4;
				enemy[enemy_counter+10].life=10000;
				enemy[enemy_counter+10].speed=timeline[time_index].speed; 
				fire_rate_arr[enemy_counter+10]=-5;     //dont shoot  
				enemy_counter++;
				
			}// case -4
				 break;
				 
			 case -5 :		   //seeker
			{
				enemy[enemy_counter+10].x=timeline[time_index].x;
				enemy[enemy_counter+10].y=timeline[time_index].y;
				enemy[enemy_counter+10].sizex=100;
				enemy[enemy_counter+10].sizey=150;
				enemy[enemy_counter+10].type=-5;
				enemy[enemy_counter+10].life=10000;
				enemy[enemy_counter+10].speed=timeline[time_index].speed; 
				fire_rate_arr[enemy_counter+10]=-5;     //dont shoot  
				enemy_counter++;
				 
			}// case -5
				 break;
				 
				 
		 case 98 :		   //primary objetive completed
			{
			BASS_ChannelPlay(primery_completed_SND,TRUE);
			timeline[time_index+10].type=99;
			timeline[time_index+450].type=100;
			timeline[time_index+600].type=101; 
	
			}// case 98
			break;
		 case 99 :		   //finish sound
			{
			BASS_ChannelPlay(finish_SND,TRUE);
		
			}// case 99
				 break;
				 
		case 100 :		   //launch finnish
			{
			finnish_launch=1;
		
			}// case 101
			break;
			
		case 101 :		   //stop the timer
			{
			total_score=total_score+score;
				Keys_Down[38]=0;
			Keys_Down[40]=0;
			Keys_Down[37]=0;
			Keys_Down[39]=0;
			
			SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 0);
		
			DisplayPanel (panelHandle_2);
			SetCtrlVal (panelHandle_2, PANEL_2_SR, score);
			SetCtrlVal (panelHandle_2, PANEL_2_TSR, total_score);
			switch (map) 
			{
				case 1:
			SetCtrlAttribute (panelHandle_2, PANEL_2_TEXTMSG, ATTR_LABEL_TEXT, "You have Finnished: Theta");
				
			break;
				case 2:
					SetCtrlAttribute (panelHandle_2, PANEL_2_TEXTMSG, ATTR_LABEL_TEXT, "You have Finnished: Vengeance");
					break;
				case 3: {
					SetCtrlAttribute (panelHandle_2, PANEL_2_TEXTMSG, ATTR_LABEL_TEXT, "You have Finnished: The Void");  
					SetCtrlAttribute (panelHandle_2, PANEL_2_EXITMAINMENU, ATTR_VISIBLE, 0); 
					insert_score(); 
					SetCtrlAttribute (panelHandle_2, PANEL_2_COMMANDBUTTON, ATTR_LABEL_TEXT, "Continue");
				}
					break;
			}
		
		
			}// case 101
			
			  break;
			
			
				case 150 :		   //explotion
			{
				kill_effect[effect].type=4;
				kill_effect[effect].x=x-(rand() % 100)+(rand() % 100);
				kill_effect[effect].y=y-50-rand() % 100;
				kill_effect[effect].time=time_index;
				effect++;
				  SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 1);
		
		
			}// case 150
				 break;
				 
			case 151 :		   //kill_stop
			{
				  DisplayPanel (panelHandle_10);
				  SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 0);
				  	Keys_Down[38]=0;
			Keys_Down[40]=0;
			Keys_Down[37]=0;
			Keys_Down[39]=0;
			wplock[1]=0;
			wplock[2]=0;
			wplock[3]=0;
			wplock[4]=0;
			SetCtrlAttribute (panelHandle, PANEL_TWO, ATTR_VISIBLE, 0);
			SetCtrlAttribute (panelHandle, PANEL_THREE, ATTR_VISIBLE, 0);
			SetCtrlAttribute (panelHandle, PANEL_FOUR, ATTR_VISIBLE, 0);
		
			}// case 151
				 break;
				 
				case 200 :		   //boss aproche
			{
			//	  boss=1;
			SetCtrlAttribute (panelHandle, PANEL_BOSSLIFE, ATTR_VISIBLE, 1);
			dir=0;
				timeline[time_index+370].type=201; 
				BASS_ChannelPlay(primery_ahead_SND,TRUE); 
				enemy[enemy_counter+10].x=w/2;
				enemy[enemy_counter+10].y=-200;
				enemy[enemy_counter+10].sizex=300;
				enemy[enemy_counter+10].sizey=300;
				enemy[enemy_counter+10].type=200;
				enemy[enemy_counter+10].life=100000;
				enemy[enemy_counter+10].speed=timeline[time_index].speed; 
				fire_rate_arr[enemy_counter+10]=-5;     //dont shoot  
				enemy_counter++;
			 
			}// case 200
				 break;
				 
			
				 
				case 201 :		   //boss activate
			{
				  boss=1;
				  dir=1;
				timeline[time_index+200].type=202;
				timeline[time_index+500].type=203;
				enemy[enemy_counter+9].life=3000;
				enemy[enemy_counter+9].speed=0; 
				enemy[enemy_counter+9].weapon=6;
				fire_rate_arr[enemy_counter+9]=10; 
				weapon_struct[enemy[enemy_counter+9].weapon].speed=-10;
				weapon_struct[enemy[enemy_counter+9].weapon].speedx=0; 
				weapon_struct[enemy[enemy_counter+9].weapon].damage=10;
			
		  
			}// case 201
			break; 
			
			case 202 :		   //boss stop firing
			{
				if (x>(enemy[enemy_counter+9].x))
				
					timeline[time_index+1].type=206;  
				else   
					timeline[time_index+1].type=207;   
				fire_rate_arr[enemy_counter+9]=-5;
				weapon_struct[enemy[enemy_counter+9].weapon].speed=-10;
				weapon_struct[enemy[enemy_counter+9].weapon].speedx=0;
			
			}// case 201
				 break;
				case 203 :		   //boss firing #1
			{
					if (x>(enemy[enemy_counter+9].x))
				
					timeline[time_index+1].type=206;  
				else   
					timeline[time_index+1].type=207;
				timeline[time_index+300].type=202;
				timeline[time_index+500].type=204;
				enemy[enemy_counter+9].doubleflag=0;	  //make the double shot
				weapon_struct[enemy[enemy_counter+9].weapon].doubleflag=0;    //make the double shot 
				enemy[enemy_counter+9].weapon=6;
				fire_rate_arr[enemy_counter+9]=10; 
				weapon_struct[enemy[enemy_counter+9].weapon].speed=-10;
				weapon_struct[enemy[enemy_counter+9].weapon].speedx=0; 
				weapon_struct[enemy[enemy_counter+9].weapon].damage=10;
			
			}// case 203
				 break;
				 
			case 204 :		   //boss firing #2
			{
				
					if (x>(enemy[enemy_counter+9].x))
				
					timeline[time_index+1].type=206;  
				else   
					timeline[time_index+1].type=207;
				timeline[time_index+300].type=202;
				timeline[time_index+500].type=205;
				enemy[enemy_counter+9].weapon=6;
				fire_rate_arr[enemy_counter+9]=10; 
				weapon_struct[enemy[enemy_counter+9].weapon].speed=-10;
				weapon_struct[enemy[enemy_counter+9].weapon].speedx=0; 
				weapon_struct[enemy[enemy_counter+9].weapon].damage=10; 
				enemy[enemy_counter+9].doubleflag=2;	  //make the double shot
				weapon_struct[enemy[enemy_counter+9].weapon].doubleflag=2;    //make the double shot 
				
			}// case 204
				 break;
					  	
				 
				case 205 :		   //boss firing #3
			{
			
					if (x>(enemy[enemy_counter+9].x))
				
					timeline[time_index+1].type=206;  
				else   
					timeline[time_index+1].type=207;
				timeline[time_index+300].type=202;
				timeline[time_index+500].type=203;
				enemy[enemy_counter+9].weapon=6;
				fire_rate_arr[enemy_counter+9]=10; 
				weapon_struct[enemy[enemy_counter+9].weapon].speed=-10;
				weapon_struct[enemy[enemy_counter+9].weapon].speedx=5; 
				weapon_struct[enemy[enemy_counter+9].weapon].damage=10; 
				enemy[enemy_counter+9].doubleflag=1;	  //make the double shot
				weapon_struct[enemy[enemy_counter+9].weapon].doubleflag=1;    //make the double shot 
			
			}// case 205
				 break;
				 
					case 206 :		   //boss  move right
			{
					enemy[enemy_counter+9].x=enemy[enemy_counter+9].x+1;
					enemy[enemy_counter+9].speed=1;
				
			
			}// case 206
				 break;
				 
			case 207 :		   //boss  move left
			{		
				enemy[enemy_counter+9].x=enemy[enemy_counter+9].x-1; 
					enemy[enemy_counter+9].speed=-1;
					
			
			}// case 207
				 break;
				 
				 
				case 250 :		   //explotion boss
			{
				kill_effect[effect].type=4;
				kill_effect[effect].x=enemy[enemy_counter+9].x-(rand() % 200)+(rand() % 200)+(w/2);
				kill_effect[effect].y=enemy[enemy_counter+9].y-(rand() % 200)+(rand() % 200)+(h/2);
				kill_effect[effect].time=time_index;
				effect++;
				  SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 1);
		
		
			}// case 150
				 break;
	}  //switch

	
	
}// time line

void kill_hit_effect() 
{
	
	effect_flag=0;
	for (c=0; c<effect; c++)
	{
		switch (kill_effect[c].type)
		{
			case 1:
				{		   
					if ((time_index < (kill_effect[c].time+10)))
						{
							effect_flag=1;
							CanvasDrawBitmap (panelHandle, PANEL_CANVAS, effect_id[0], VAL_ENTIRE_OBJECT,MakeRect(kill_effect[c].y,kill_effect[c].x,25,25)); 	
						}
						else
							if  ((time_index < (kill_effect[c].time+20))) 
							{
							CanvasDrawBitmap (panelHandle, PANEL_CANVAS, effect_id[1], VAL_ENTIRE_OBJECT,MakeRect(kill_effect[c].y,kill_effect[c].x,25,25)); 
							effect_flag=1; 
						}
							else
							if  ((time_index < (kill_effect[c].time+30))) 
							{
							CanvasDrawBitmap (panelHandle, PANEL_CANVAS, effect_id[2], VAL_ENTIRE_OBJECT,MakeRect(kill_effect[c].y,kill_effect[c].x,25,25)); 
							effect_flag=1; 
						}
				}
				break;
				
			case 2:
				{		   
					if ((time_index < (kill_effect[c].time+10)))
						{
							effect_flag=1;
							CanvasDrawBitmap (panelHandle, PANEL_CANVAS, effect_id[0], VAL_ENTIRE_OBJECT,MakeRect(kill_effect[c].y,kill_effect[c].x,25,25)); 	
						}
						else
							if  ((time_index < (kill_effect[c].time+20))) 
							{
							CanvasDrawBitmap (panelHandle, PANEL_CANVAS, effect_id[1], VAL_ENTIRE_OBJECT,MakeRect(kill_effect[c].y,kill_effect[c].x,25,25));
							effect_flag=1; 
						}
							else
							if  ((time_index < (kill_effect[c].time+30))) 
							{
							CanvasDrawBitmap (panelHandle, PANEL_CANVAS, effect_id[2], VAL_ENTIRE_OBJECT,MakeRect(kill_effect[c].y,kill_effect[c].x,25,25)); 
							effect_flag=1; 
						}
					
							
				}
				break;
				
				case 3:
				{		   
					if ((time_index < (kill_effect[c].time+5)))
						{
							BASS_ChannelPlay(explosion1_SND,TRUE);
							CanvasDrawBitmap (panelHandle, PANEL_CANVAS, effect_id[3], VAL_ENTIRE_OBJECT,MakeRect(kill_effect[c].y,kill_effect[c].x,50,50)); 	
							effect_flag=1;
						}
						else
							if  ((time_index < (kill_effect[c].time+10))) 
							{
							CanvasDrawBitmap (panelHandle, PANEL_CANVAS, effect_id[4], VAL_ENTIRE_OBJECT,MakeRect(kill_effect[c].y,kill_effect[c].x,50,50));
							effect_flag=1;
						}
							else
							if  ((time_index < (kill_effect[c].time+15))) 
							{
							CanvasDrawBitmap (panelHandle, PANEL_CANVAS, effect_id[5], VAL_ENTIRE_OBJECT,MakeRect(kill_effect[c].y,kill_effect[c].x,50,50));
							effect_flag=1;
						}
						else
							if  ((time_index < (kill_effect[c].time+20))) 
							{
							CanvasDrawBitmap (panelHandle, PANEL_CANVAS, effect_id[6], VAL_ENTIRE_OBJECT,MakeRect(kill_effect[c].y,kill_effect[c].x,50,50));
							effect_flag=1;
						}
						else
							if  ((time_index < (kill_effect[c].time+25))) 
							{
							CanvasDrawBitmap (panelHandle, PANEL_CANVAS, effect_id[7], VAL_ENTIRE_OBJECT,MakeRect(kill_effect[c].y,kill_effect[c].x,40,40));
							effect_flag=1;
						}
					
							
				}
				break;
				
				
			case 4:
				{		   
					if ((time_index < (kill_effect[c].time+5)))
						{
							
							BASS_ChannelPlay(explosion1_SND,TRUE);
							CanvasDrawBitmap (panelHandle, PANEL_CANVAS, effect_id[3], VAL_ENTIRE_OBJECT,MakeRect(kill_effect[c].y,kill_effect[c].x,100,100)); 	
							effect_flag=1;
						}
						else
							if  ((time_index < (kill_effect[c].time+10))) 
							{
							CanvasDrawBitmap (panelHandle, PANEL_CANVAS, effect_id[4], VAL_ENTIRE_OBJECT,MakeRect(kill_effect[c].y,kill_effect[c].x,100,100));
							effect_flag=1;
						}
							else
							if  ((time_index < (kill_effect[c].time+15))) 
							{
							CanvasDrawBitmap (panelHandle, PANEL_CANVAS, effect_id[5], VAL_ENTIRE_OBJECT,MakeRect(kill_effect[c].y,kill_effect[c].x,100,100));
							effect_flag=1;
						}
						else
							if  ((time_index < (kill_effect[c].time+20))) 
							{
							CanvasDrawBitmap (panelHandle, PANEL_CANVAS, effect_id[6], VAL_ENTIRE_OBJECT,MakeRect(kill_effect[c].y,kill_effect[c].x,100,100));
							effect_flag=1;
						}
						else
							if  ((time_index < (kill_effect[c].time+25))) 
							{
							CanvasDrawBitmap (panelHandle, PANEL_CANVAS, effect_id[7], VAL_ENTIRE_OBJECT,MakeRect(kill_effect[c].y,kill_effect[c].x,100,100));
							effect_flag=1;
						}
					
							
				}
				break;
		}//switch
	}//for

}// kill hit effect

void find_target_lock()
{
	if (lock_missile.flag==0)
	{
		for (i_enemy=10; i_enemy<enemy_counter+10;i_enemy++)
		{ 
			if ((enemy[i_enemy].life>0) && (enemy[i_enemy].type != -1) ) 
			{
				lock_missile.index=i_enemy;
				lock_missile.flag=1;
			}//life
		
		}//for look
	}//flag
	
} // find target to lock


void write_mssg()
{
	switch (mssg_flag) 
	{
		case 1:
			SetCtrlVal (panelHandle, PANEL_TEXTMSG, "Energy Gun Selected");
			break;
	}			//end switch
}//end write mssg
					
					


int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "project_v2.uir", PANEL)) < 0)
		return -1;
	if ((panelHandle_2 = LoadPanel (0, "project_v2.uir", PANEL_2)) < 0)
        return -1;
	if ((panelHandle_3 = LoadPanel (0, "project_v2.uir", PANEL_3)) < 0)
        return -1;
	if ((panelHandle_4 = LoadPanel (0, "project_v2.uir", PANEL_4)) < 0)
        return -1;
	if ((panelHandle_5 = LoadPanel (0, "project_v2.uir", PANEL_5)) < 0)
        return -1;
	if ((panelHandle_6 = LoadPanel (0, "project_v2.uir", PANEL_6)) < 0)
        return -1;
	if ((panelHandle_7 = LoadPanel (0, "project_v2.uir", PANEL_7)) < 0)
        return -1;
	if ((panelHandle_8 = LoadPanel (0, "project_v2.uir", PANEL_8)) < 0)
        return -1;
	if ((panelHandle_9 = LoadPanel (0, "project_v2.uir", PANEL_9)) < 0)
        return -1;
	if ((panelHandle_10 = LoadPanel (0, "project_v2.uir", PANEL_10)) < 0)
        return -1;
	BASS_Init( -1,44100, 0,0,NULL); 
	setup();
	Connect_To_Windows_Events();  //asking windows to send keyboard press and release events to our software
	DisplayPanel (panelHandle_4); //main menu
	RunUserInterface ();
	DiscardPanel (panelHandle);
	DiscardPanel (panelHandle_2);
	DiscardPanel (panelHandle_3);
	DiscardPanel (panelHandle_4);
	DiscardPanel (panelHandle_5);
	DiscardPanel (panelHandle_6);
	DiscardPanel (panelHandle_7);
	DiscardPanel (panelHandle_8);
	DiscardPanel (panelHandle_9);
	DiscardPanel (panelHandle_10);
	return 0;
}

int CVICALLBACK panelFun (int panel, int event, void *callbackData,
						  int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface (0);
			break;
	}
	return 0;
}


int CVICALLBACK panelFunc2 (int panel, int event, void *callbackData,
							int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface (0);

			break;
	}
	return 0;
}


//Timer is monitoring pressed keys
int CVICALLBACK tick (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			counter=counter+0.01;
		
			time_index++;
			music_player(); //make sure the correct music with reply 
			CanvasStartBatchDraw (panelHandle, PANEL_CANVAS);
			
				if (Keys_Down[17]==1) {
				weapon=pweapon;//fire 
				if (weapon_struct[weapon].clip!=0)
				fire_rate_func(); //rate of fire
				}

			if (Keys_Down[27]==1) {  //Esc
				
			Keys_Down[38]=0;
			Keys_Down[40]=0;
			Keys_Down[37]=0;
			Keys_Down[39]=0;
		
				
				
			SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 0);
			DisplayPanel (panelHandle_9);
			Keys_Down[27]=0;
			}//Esc
			enemyfire();
			  
			movefire();  // move all shots
			if (finnish_launch==1){
				y=y-11;}
			
			switchweapon(); //change radix weapon 
			  //make it back to radix weapon
			backx=backx+0.5;
			if (backx == h)
				backx = 0;
			CanvasDrawBitmap (panelHandle, PANEL_CANVAS, back1_id, VAL_ENTIRE_OBJECT, MakeRect (backx, 0, h, w)); //backgorund
			CanvasDrawBitmap (panelHandle, PANEL_CANVAS, back1_id, VAL_ENTIRE_OBJECT, MakeRect (backx-h, 0, h, w));   //backgorund 
			collision(); //shots hits the enemy 
			drawshots(); //draw all shots 
			if (shot_flag==0) //clear the shot buffer	 
				{
					i=1;
					j=1;			  
				}
			time_line(); //time line
			draw_kill_enemy();   //draw the enemy and kill them
			if (enemy_flag==0) //clear the enemy buffer	 
				{
					enemy_counter=0;
					
				}
			kill_hit_effect();
			if (effect_flag==0) //clear the enemy buffer	 
				{
					effect=0;
					
				}
			find_target_lock(); 
			if ( lock_missile.flag==1 && pweapon==4) 
			{
			
				CanvasDrawRect (panelHandle, PANEL_CANVAS, MakeRect(enemy[lock_missile.index].y-(enemy[lock_missile.index].sizey/2),enemy[lock_missile.index].x-(enemy[lock_missile.index].sizex/2),enemy[lock_missile.index].sizex+10,enemy[lock_missile.index].sizex+10), VAL_DRAW_FRAME); //make the lock missile rectangle
			}
			
			
		
			if	(Keys_Down[39]==0 && Keys_Down[37]==0)
			 
			{
	    		CanvasDrawBitmap (panelHandle, PANEL_CANVAS, radix_id, VAL_ENTIRE_OBJECT,MakeRect(y-80,x-50,100,100)); //radix
			}
			if	(x>(w-10) || x==10)
			 
			{
	    		CanvasDrawBitmap (panelHandle, PANEL_CANVAS, radix_id, VAL_ENTIRE_OBJECT,MakeRect(y-80,x-50,100,100)); //radix
			}
			speedradix();
			CanvasEndBatchDraw (panelHandle, PANEL_CANVAS);						
									
			break;
		
	}
	return 0;
}


int CVICALLBACK continue_next_map (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			finnish_launch=0;
			map++;
			if (map<4)
			{
			load_map(); 
			 
			DisplayPanel (panelHandle);
			 HidePanel (panelHandle_2);
			 SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 1);
			 
			}//is not map 4
			else 
			{
				make_table();
				HidePanel (panelHandle_2);
				DisplayPanel (panelHandle_3);
				
				
				
				
			}
			break;
	}
	return 0;
}

int CVICALLBACK exit_main_menu (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			BASS_ChannelStop(music_SND);
			BASS_ChannelStop(music2_SND);
			BASS_ChannelStop(music3_SND);
	; 
			HidePanel (panelHandle_2);
			HidePanel (panelHandle_3);
			HidePanel (panelHandle);
			DisplayPanel (panelHandle_4);

			break;
	}
	return 0;
}

int CVICALLBACK panelFunc3 (int panel, int event, void *callbackData,
							int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface (0);

			break;
	}
	return 0;
}

int CVICALLBACK panelFunc4 (int panel, int event, void *callbackData,
							int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface (0); 

			break;
	}
	return 0;
}

int CVICALLBACK newgame (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetCtrlVal (panelHandle_8, PANEL_8_STRINGNAME, "");
			 BASS_ChannelPlay(buttonclickrelease0005_SND,TRUE); 
			DisplayPanel (panelHandle_8); 
	

			break;
			
		case EVENT_MOUSE_POINTER_MOVE:
			
			if (flag_menu_sound[0]==0)		//prevent the sound to reply itself
				{
			 		BASS_ChannelPlay(buttonrollover_SND,TRUE);
					SetCtrlAttribute (panelHandle_4, PANEL_4_NEWGAME, ATTR_LABEL_COLOR, VAL_GRAY);
					SetCtrlAttribute (panelHandle_4, PANEL_4_OPTIONS, ATTR_LABEL_COLOR, VAL_WHITE);
					SetCtrlAttribute (panelHandle_4, PANEL_4_ABOUT, ATTR_LABEL_COLOR, VAL_WHITE); 
					SetCtrlAttribute (panelHandle_4, PANEL_4_HELP, ATTR_LABEL_COLOR, VAL_WHITE); 
					SetCtrlAttribute (panelHandle_4, PANEL_4_DEMO, ATTR_LABEL_COLOR, VAL_WHITE); 
					SetCtrlAttribute (panelHandle_4, PANEL_4_QUIT, ATTR_LABEL_COLOR, VAL_WHITE); 
					SetCtrlAttribute (panelHandle_4, PANEL_4_TOPTEN, ATTR_LABEL_COLOR, VAL_WHITE);
					flag_menu_sound[0]=1;
					flag_menu_sound[1]=0;
					flag_menu_sound[2]=0;
					flag_menu_sound[3]=0;
					flag_menu_sound[4]=0;
					flag_menu_sound[5]=0;
					flag_menu_sound[6]=0;
			
						
				}
			
			break;
			
			
			
	}
	return 0;
}

int CVICALLBACK options (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			 BASS_ChannelPlay(buttonclickrelease0005_SND,TRUE); 
			SetCtrlVal (panelHandle_7, PANEL_7_NUMERICSLIDE,vol_music);
			SetCtrlVal (panelHandle_7, PANEL_7_NUMERICSLIDE_2,vol_effect);
			DisplayPanel (panelHandle_7); 

			break;
			
		case EVENT_MOUSE_POINTER_MOVE:
			
			if (flag_menu_sound[1]==0)		//prevent the sound to reply itself
				{
			 		BASS_ChannelPlay(buttonrollover_SND,TRUE);
					flag_menu_sound[0]=0;
					flag_menu_sound[1]=1;
					flag_menu_sound[2]=0;
					flag_menu_sound[3]=0;
					flag_menu_sound[4]=0;
					flag_menu_sound[5]=0;
					flag_menu_sound[6]=0;
					SetCtrlAttribute (panelHandle_4, PANEL_4_NEWGAME, ATTR_LABEL_COLOR, VAL_WHITE);
					SetCtrlAttribute (panelHandle_4, PANEL_4_OPTIONS, ATTR_LABEL_COLOR, VAL_GRAY);
					SetCtrlAttribute (panelHandle_4, PANEL_4_ABOUT, ATTR_LABEL_COLOR, VAL_WHITE); 
					SetCtrlAttribute (panelHandle_4, PANEL_4_HELP, ATTR_LABEL_COLOR, VAL_WHITE); 
					SetCtrlAttribute (panelHandle_4, PANEL_4_DEMO, ATTR_LABEL_COLOR, VAL_WHITE); 
					SetCtrlAttribute (panelHandle_4, PANEL_4_QUIT, ATTR_LABEL_COLOR, VAL_WHITE); 
					SetCtrlAttribute (panelHandle_4, PANEL_4_TOPTEN, ATTR_LABEL_COLOR, VAL_WHITE);
					
			
						
				}
			
			break;
	}
	return 0;
}

int CVICALLBACK about (int panel, int control, int event,
					   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			 BASS_ChannelPlay(buttonclickrelease0005_SND,TRUE); 
			DisplayPanel (panelHandle_5);

			break;
			
				case EVENT_MOUSE_POINTER_MOVE:
			
			if (flag_menu_sound[2]==0)		//prevent the sound to reply itself
				{
			 		BASS_ChannelPlay(buttonrollover_SND,TRUE);
					flag_menu_sound[0]=0;
					flag_menu_sound[1]=0;
					flag_menu_sound[2]=1;
					flag_menu_sound[3]=0;
					flag_menu_sound[4]=0;
					flag_menu_sound[5]=0;
					flag_menu_sound[6]=0;
					SetCtrlAttribute (panelHandle_4, PANEL_4_NEWGAME, ATTR_LABEL_COLOR, VAL_WHITE);
					SetCtrlAttribute (panelHandle_4, PANEL_4_OPTIONS, ATTR_LABEL_COLOR, VAL_WHITE);
					SetCtrlAttribute (panelHandle_4, PANEL_4_ABOUT, ATTR_LABEL_COLOR, VAL_GRAY); 
					SetCtrlAttribute (panelHandle_4, PANEL_4_HELP, ATTR_LABEL_COLOR, VAL_WHITE); 
					SetCtrlAttribute (panelHandle_4, PANEL_4_DEMO, ATTR_LABEL_COLOR, VAL_WHITE); 
					SetCtrlAttribute (panelHandle_4, PANEL_4_QUIT, ATTR_LABEL_COLOR, VAL_WHITE);
					SetCtrlAttribute (panelHandle_4, PANEL_4_TOPTEN, ATTR_LABEL_COLOR, VAL_WHITE);
			
						
				}
			
			break;
	}
	return 0;
}

int CVICALLBACK panelFunc5 (int panel, int event, void *callbackData,
							int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			flag_menu_sound[2]=0;
			HidePanel (panelHandle_5);

			break;
	}
	return 0;
}

int CVICALLBACK exitabout (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			flag_menu_sound[2]=0;
			HidePanel (panelHandle_5);

			break;
	}
	return 0;
}

int CVICALLBACK quitgame (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			QuitUserInterface (0);

			break;
	}
	return 0;
}

int CVICALLBACK cancelexit (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			flag_menu_sound[5]=0;
			HidePanel (panelHandle_6); 

			break;
	}
	return 0;
}

int CVICALLBACK panelFunc6 (int panel, int event, void *callbackData,
							int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			flag_menu_sound[5]=0;
			HidePanel (panelHandle_6);

			break;
	}
	return 0;
}

int CVICALLBACK quit (int panel, int control, int event,
					  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			 BASS_ChannelPlay(buttonclickrelease0005_SND,TRUE); 
			 
			DisplayPanel (panelHandle_6);

			break;
			
			case EVENT_MOUSE_POINTER_MOVE:
			
			if (flag_menu_sound[5]==0)		//prevent the sound to reply itself
				{
			 		BASS_ChannelPlay(buttonrollover_SND,TRUE);
					flag_menu_sound[0]=0;
					flag_menu_sound[1]=0;
					flag_menu_sound[2]=0;
					flag_menu_sound[3]=0;
					flag_menu_sound[4]=0;
					flag_menu_sound[5]=1;
					flag_menu_sound[6]=0;
					SetCtrlAttribute (panelHandle_4, PANEL_4_NEWGAME, ATTR_LABEL_COLOR, VAL_WHITE);
					SetCtrlAttribute (panelHandle_4, PANEL_4_OPTIONS, ATTR_LABEL_COLOR, VAL_WHITE);
					SetCtrlAttribute (panelHandle_4, PANEL_4_ABOUT, ATTR_LABEL_COLOR, VAL_WHITE); 
					SetCtrlAttribute (panelHandle_4, PANEL_4_HELP, ATTR_LABEL_COLOR, VAL_WHITE); 
					SetCtrlAttribute (panelHandle_4, PANEL_4_DEMO, ATTR_LABEL_COLOR, VAL_WHITE); 
					SetCtrlAttribute (panelHandle_4, PANEL_4_QUIT, ATTR_LABEL_COLOR, VAL_GRAY);
					SetCtrlAttribute (panelHandle_4, PANEL_4_TOPTEN, ATTR_LABEL_COLOR, VAL_WHITE);
			
						
				}
				break;
	}
	return 0;
}

int CVICALLBACK panelFunc7 (int panel, int event, void *callbackData,
							int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			flag_menu_sound[1]=0;
			HidePanel (panelHandle_7); 

			break;
	}
	return 0;
}

int CVICALLBACK ok (int panel, int control, int event,   //ok options
					void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			flag_menu_sound[1]=0;
			GetCtrlVal (panelHandle_7, PANEL_7_NUMERICSLIDE,&vol_music);
			GetCtrlVal (panelHandle_7, PANEL_7_NUMERICSLIDE_2,&vol_effect);
			BASS_ChannelSetAttribute(buttonclickrelease0005_SND, BASS_ATTRIB_VOL, vol_effect);
			BASS_ChannelSetAttribute(buttonrollover_SND, BASS_ATTRIB_VOL, vol_effect);
			HidePanel (panelHandle_7);
			
			break;
	}
	return 0;
}

int CVICALLBACK canel (int panel, int control, int event,
					   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			flag_menu_sound[1]=0;
			HidePanel (panelHandle_7);

			break;
	}
	return 0;
}

int CVICALLBACK panelFunc8 (int panel, int event, void *callbackData,
							int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			flag_menu_sound[0]=0;
			HidePanel (panelHandle_8);

			break;
	}
	return 0;
}

int CVICALLBACK strart_game_button (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panelHandle_8, PANEL_8_STRINGNAME, name);
		
			SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 1);
			HidePanel (panelHandle_4);
			HidePanel (panelHandle_8);
			DisplayPanel (panelHandle);
			start_new_game() ; //start new game function

			break;
	}
	return 0;
}

int CVICALLBACK help (int panel, int control, int event,
					  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			 BASS_ChannelPlay(buttonclickrelease0005_SND,TRUE); 
			 OpenDocumentInDefaultViewer("Help.pdf",VAL_NO_ZOOM);

			break;
			
				case EVENT_MOUSE_POINTER_MOVE:
			
			if (flag_menu_sound[3]==0)		//prevent the sound to reply itself
				{
			 		BASS_ChannelPlay(buttonrollover_SND,TRUE);
					flag_menu_sound[0]=0;
					flag_menu_sound[1]=0;
					flag_menu_sound[2]=0;
					flag_menu_sound[3]=1;
					flag_menu_sound[4]=0;
					flag_menu_sound[5]=0;
					flag_menu_sound[6]=0;
					SetCtrlAttribute (panelHandle_4, PANEL_4_NEWGAME, ATTR_LABEL_COLOR, VAL_WHITE);
					SetCtrlAttribute (panelHandle_4, PANEL_4_OPTIONS, ATTR_LABEL_COLOR, VAL_WHITE);
					SetCtrlAttribute (panelHandle_4, PANEL_4_ABOUT, ATTR_LABEL_COLOR, VAL_WHITE); 
					SetCtrlAttribute (panelHandle_4, PANEL_4_HELP, ATTR_LABEL_COLOR, VAL_GRAY); 
					SetCtrlAttribute (panelHandle_4, PANEL_4_DEMO, ATTR_LABEL_COLOR, VAL_WHITE); 
					SetCtrlAttribute (panelHandle_4, PANEL_4_QUIT, ATTR_LABEL_COLOR, VAL_WHITE);
					SetCtrlAttribute (panelHandle_4, PANEL_4_TOPTEN, ATTR_LABEL_COLOR, VAL_WHITE);
			
						
				}
			
			break;
	}
	return 0;
}

int CVICALLBACK demo (int panel, int control, int event,
					  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			 BASS_ChannelPlay(buttonclickrelease0005_SND,TRUE); 
			 InetLaunchDefaultWebBrowser ("https://youtu.be/_HcisfnySlg");

			break;
			
			
				case EVENT_MOUSE_POINTER_MOVE:
			
			if (flag_menu_sound[4]==0)		//prevent the sound to reply itself
				{
			 		BASS_ChannelPlay(buttonrollover_SND,TRUE);
					flag_menu_sound[0]=0;
					flag_menu_sound[1]=0;
					flag_menu_sound[2]=0;
					flag_menu_sound[3]=0;
					flag_menu_sound[4]=1;
					flag_menu_sound[5]=0;
					flag_menu_sound[6]=0;
					SetCtrlAttribute (panelHandle_4, PANEL_4_NEWGAME, ATTR_LABEL_COLOR, VAL_WHITE);
					SetCtrlAttribute (panelHandle_4, PANEL_4_OPTIONS, ATTR_LABEL_COLOR, VAL_WHITE);
					SetCtrlAttribute (panelHandle_4, PANEL_4_ABOUT, ATTR_LABEL_COLOR, VAL_WHITE); 
					SetCtrlAttribute (panelHandle_4, PANEL_4_HELP, ATTR_LABEL_COLOR, VAL_WHITE); 
					SetCtrlAttribute (panelHandle_4, PANEL_4_DEMO, ATTR_LABEL_COLOR, VAL_GRAY); 
					SetCtrlAttribute (panelHandle_4, PANEL_4_QUIT, ATTR_LABEL_COLOR, VAL_WHITE);
					SetCtrlAttribute (panelHandle_4, PANEL_4_TOPTEN, ATTR_LABEL_COLOR, VAL_WHITE);
			
						
				}
				break;
				
	}		   	
	return 0;
}

int CVICALLBACK panelFunc9 (int panel, int event, void *callbackData,
							int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 1); 
			HidePanel (panelHandle_9);

			break;
	}
	return 0;
}

int CVICALLBACK cancelp9 (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 1);
			HidePanel (panelHandle_9);

			break;
	}
	return 0;
}

int CVICALLBACK topten (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			 BASS_ChannelPlay(buttonclickrelease0005_SND,TRUE);
			 DisplayPanel (panelHandle_3);

			break;
			
			
				case EVENT_MOUSE_POINTER_MOVE:
			
			if (flag_menu_sound[6]==0)		//prevent the sound to reply itself
				{
			 		BASS_ChannelPlay(buttonrollover_SND,TRUE);
					 make_table(); 
					flag_menu_sound[0]=0;
					flag_menu_sound[1]=0;
					flag_menu_sound[2]=0;
					flag_menu_sound[3]=0;
					flag_menu_sound[4]=0;
					flag_menu_sound[5]=0;
					flag_menu_sound[6]=1;
					SetCtrlAttribute (panelHandle_4, PANEL_4_NEWGAME, ATTR_LABEL_COLOR, VAL_WHITE);
					SetCtrlAttribute (panelHandle_4, PANEL_4_OPTIONS, ATTR_LABEL_COLOR, VAL_WHITE);
					SetCtrlAttribute (panelHandle_4, PANEL_4_ABOUT, ATTR_LABEL_COLOR, VAL_WHITE); 
					SetCtrlAttribute (panelHandle_4, PANEL_4_HELP, ATTR_LABEL_COLOR, VAL_WHITE); 
					SetCtrlAttribute (panelHandle_4, PANEL_4_DEMO, ATTR_LABEL_COLOR, VAL_WHITE); 
					SetCtrlAttribute (panelHandle_4, PANEL_4_QUIT, ATTR_LABEL_COLOR, VAL_WHITE);
					SetCtrlAttribute (panelHandle_4, PANEL_4_TOPTEN, ATTR_LABEL_COLOR, VAL_GRAY);
			
						
				}
				break;
				
	}		   	
	return 0;
}

int CVICALLBACK panelFunc10 (int panel, int event, void *callbackData,
							 int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			BASS_ChannelStop(music_SND);
			BASS_ChannelStop(music2_SND);
			BASS_ChannelStop(music3_SND);
			HidePanel (panelHandle_2);
			HidePanel (panelHandle_3);
			HidePanel (panelHandle);
			HidePanel (panelHandle_10);
			DisplayPanel (panelHandle_4);

			break;
	}
	return 0;
}

int CVICALLBACK replay (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			life=100;
			SetCtrlVal (panelHandle, PANEL_HEATH, 100);
			load_map();
			SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 1); 
			HidePanel (panelHandle_10);

			break;
	}
	return 0;
}
																																		  
