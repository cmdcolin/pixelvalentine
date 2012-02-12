#include "misc.h"

int scrx = 640;
int scry = 480;

int updates_per_second = 20;
int fps_per_update = 10;

#define num_hearts 30
#define num_clouds 7
int fieldw = 30;
int fieldh = 20;

    struct xyz {
        float x, y, z;
    };


int main() 
{
    int width, height;
    char c;
    int ret;
    int i;

    int fps = 0;
    int frames_done = 0;
    int old_time = 0;

    int frames_array[10]; //an array to store the number of frames we did during the last 10 tenths of a second
    int frame_index = 0; //used to store the index of the last updated value in the array

    DATAFILE * d;
    FONT * fonty;
    BITMAP *heart,*buffer,*bg,*cloud,*sun,*moon;

    int cloud_mover = 0;
    float sunset = 0.0f;
    float moonrise = 0.0f;
    float sunset_speed = 0.1f;
    float moon_speed = 0.0f;

    struct rgb_color bgcolor;
    struct rgb_color suncolor;

        struct xyz p[num_hearts]; 
        struct xyz p2[num_hearts]; 
        struct xyz cl[num_clouds];

    MATRIX_f mf_retard;
    MATRIX_f mf_ok;

    int hang_out = 0;
    int day = 1;
    int nocheck=0;

    float theta = 0.0;
    const float theta_speed = 0.05;

    float cam = -50;
    float camfly = 0.8;


    bgcolor.r = 30;    bgcolor.ro = 30;
    bgcolor.g = 60;    bgcolor.go = 60;
    bgcolor.b = 150;    bgcolor.bo = 150;

    suncolor.r = 255;    suncolor.ro = 255;
    suncolor.g = 200;    suncolor.go = 200;
    suncolor.b = 0;    suncolor.bo = 0;



    setup();

    printf("operating system   = %s\n", os_name(os_type));
    printf("allegro version    = %s\n", allegro_id);
    printf("os version         = %d.%d\n", os_version, os_revision);

    if (get_desktop_resolution(&width, &height) != 0)
        printf("error reading desktop resolution\n");

    printf("desktop resolution = %d x %d\n", width, height);
    printf("color depth        = %d\n", desktop_color_depth());


        set_color_depth(32);
        ret =set_gfx_mode(GFX_AUTODETECT, scrx, scry, 0, 0);
        if(ret < 0)
            printf("Error: %s", allegro_error);
    
    LOCK_VARIABLE(ticks);
    //LOCK_FUNCTION(ticker);
    install_int_ex(ticker, BPS_TO_TIMER(updates_per_second));

    LOCK_VARIABLE(game_time);
    //LOCK_FUNCTION(game_time_ticker);
    install_int_ex(game_time_ticker, BPS_TO_TIMER(fps_per_update));

    for(i = 0; i < 10; i++)
        frames_array[i] = 0;


    d = load_datafile("sprites.dat");
    if(!d)
    {
        printf("Error: %s\n", allegro_error);
        return 0;
    }

    
    heart = (BITMAP*)d[0].dat;
    bg = (BITMAP*)d[1].dat;
    cloud = (BITMAP*)d[2].dat;
    moon = (BITMAP*)d[3].dat;
    fonty = (FONT*)d[4].dat;

    //heart = load_bitmap("picture 2.bmp",0);
    //bg = load_bitmap("picture 3.bmp",0);
    //cloud = load_bitmap("picture 4.bmp",0);
    //moon = load_bitmap("picture 5.bmp",0);
    //FONT * fonty = load_font("atarifont.pcx",0,0);

    if(!heart || !bg || !cloud | !moon || !fonty)
    {
        printf("Wow:%s\n",allegro_error); 
        return 0;
    }

    buffer = create_bitmap(scrx, scry);
    sun = create_bitmap(200, 200);



    // Render to whole screen
    set_projection_viewport(0, 0, SCREEN_W, SCREEN_H);

   install_mouse();
   show_mouse(screen);


    for(i = 0; i < num_hearts; i++) 
    {
        p[i].x = rrandf(-fieldw, fieldw);
        p[i].y = rrandf(-fieldh, fieldh);
        p[i].z = rrandf(-150, 300);
    }

    for(i = 0; i < num_clouds; i++) 
    {
        cl[i].x = -20 + 40 * i / num_clouds;
        cl[i].y = 3 + rrandf(0, 4);
        cl[i].z = 0;
    }



    while(!key[KEY_ESC])
    {
        while(ticks == 0)
            rest(100 / updates_per_second);

        while(ticks > 0)
        {
            int old_ticks = ticks;

            get_camera_matrix_f(
                &mf_retard, 
                0, 0, cam,        // position
                0, 0, 1,        // look vector
                0, 1, 0,        // up vector
                32,
                (float)SCREEN_W / (float)SCREEN_H);


            get_camera_matrix_f(
                &mf_ok, 
                0, 0, -20,        // position
                0, 0, 1,        // look vector
                0, 1, 0,        // up vector
                32,
                (float)SCREEN_W / (float)SCREEN_H);

            for(i = 0; i < num_hearts; ++i)
            {
                p2[i].x = p[i].x;
                p2[i].y = p[i].y; 
                p2[i].z = p[i].z+ fabs((float)(50 * sin(1.5*theta)));
            }

            cam=75+75*sin(1.3*theta);
            theta+=theta_speed;

            if(7-sunset/5 < 2)
                process_color(&bgcolor, sunset_speed*16);

            process_color_r(&suncolor, 0,sunset_speed*16,0);
            if(7-sunset/5 < 2)
                process_color_r(&suncolor, sunset_speed*7,0,0);



            sunset += sunset_speed;
            moonrise += moon_speed;

            if(cloud_mover >= 3)
            {
                int index = rrand(0, num_clouds);
                index %= num_clouds;
                cl[index].x += 1;
                cloud_mover=0;
                if(cl[index].x > 20) cl[index].x = -20;
            }

            darken_pixel(bg, 0, 1, 0);

            ticks--;
            if(old_ticks <= ticks)
                break;
        }

        if(game_time >= old_time + 1) //i.e. a 0.1 second has passed since we last counted the frames
        {
            fps -= frames_array[frame_index]; //decrement the fps by the frames done a second ago
            frames_array[frame_index] = frames_done; //store the number of frames done this 0.1 second
            fps += frames_done; //increment the fps by the newly done frames

            frame_index = (frame_index + 1) % 10;//increment the frame index and snap it to 10
            frames_done = 0;

            old_time++;
            cloud_mover++;

            if(nocheck>0)
                nocheck--;

            /*if(nocheck==0 && fabs(sunset - 10) < 1.0){
                hang_out++;sunset_speed = 0;
            }*/
            if(nocheck==0 && fabs(sunset - 40) < 1.0){
                hang_out++;sunset_speed = 0;
            }
            if(nocheck==0 && fabs(moonrise - 5) < 1.0) {
                hang_out++;moon_speed = 0;
            }


            if(hang_out > 25)
            {
                hang_out = 0;

                if(fabs(sunset - 40) < 1.0)
                {
                    moon_speed = 0.1f;
                    sunset_speed = 0.0f;
                    nocheck = 10;
                    day = day?0:1;
                }
                if(fabs(moonrise - 5) < 1.0)
                {
                    moon_speed = 0;
                    sunset_speed = 0;
                    nocheck = 10;
                    day = day?0:1;
                }
                /*else if(fabs(sunset - 10) < 1.0)
                {
                    moon_speed = 0;
                    sunset_speed = 0.1f;
                    nocheck = 10;
                    day = day?0:1;
                }*/
            }
        }


        floodfill(buffer, 0, 0, make_color(&bgcolor));
        floodfill(sun, 0, 0, make_color(&bgcolor));
        circlefill(sun, 100, 100, 25, make_color(&suncolor));
        
        //stretch_blit(sun, buffer, 0, 0, 50, 50, 200, -50+sunset*20, 100, 100);
        render_object(&mf_ok, buffer, sun, 5-sunset/10, 7-sunset/5, -10);
        render_object(&mf_ok, buffer, moon, 1-(moonrise/3), 10-(moonrise), 0);

        for(i = 0; i < num_hearts; i++)
            render_object(&mf_retard, buffer, heart, p2[i].x, p2[i].y, p2[i].z);

        for(i = 0; i < num_clouds; i++)
            render_object(&mf_ok, buffer, cloud, cl[i].x, cl[i].y, 0);

        render_object(&mf_ok, buffer, bg, 10, -3, 0);

        //render_object(&mf_retard, buffer, bg, 10, -1.3, cam+10);
        //stretch_sprite(buffer, bg, -415, 335, 1860, 236);

        textprintf_ex(buffer, fonty, 10, 10, 
            makecol(255, 255, 255), -1, "%f\t%d", cam, fps);

        blit(buffer, screen, 0,0,0,0, scrx, scry);
        clear_bitmap(buffer);
        clear_bitmap(sun);



        frames_done++;
    }

    destroy_bitmap(buffer);
    destroy_bitmap(sun);
    
    //unload_datafile(d);

    return 0;
}
END_OF_MAIN()
