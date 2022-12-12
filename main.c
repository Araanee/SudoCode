#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <gtk/gtk.h>
#include "utils/auxiliary.h"
#include "pretreatment/rotation.h"
#include "pretreatment/auto_rotation.h"
#include "pretreatment/delete_color.h"
#include "pretreatment/grayscale.h"
#include "pretreatment/gaussian.h"
#include "pretreatment/gamma.h"
#include "pretreatment/threshold.h"
#include "pretreatment/resize.h"
#include "pretreatment/sobel.h"
#include "grid/grid_detection.h"
#include "grid/grid_row_extraction.h"
#include "grid/grid_numbers_extraction.h"
#include "reconstruction/reconstruct.h"
#include "resolution/resolution.h"
#include "resolution/import_export.h"
#include "grid_to_image/construct.h"
#include "grid_to_image/pixel_operations.h"
#include "grid_to_image/newreader.h"
#include "ocr_neural_network/predicting.h"
#include "ocr_neural_network/Neural.h"
#include "ocr_neural_network/DataLoad.h"
#include "ocr_neural_network/Persist.h"
#include "ocr_neural_network/MathTools.h"

typedef struct
{
	GtkBuilder *builder;
      	gpointer user_data;
} SGlobalData;

int* predictions;
char *filename_img;
void callback_solve(GtkButton *button, gpointer user_data);
void callback_loadbutton(GtkButton *button, gpointer user_data);
void callback_savebutton(GtkButton *button, gpointer user_data);
//void on_toggle1_toggled(GtkToggleButton *b,GtkBuilder builder);  
void rotate_img(GtkButton *button, gpointer user_data);
void on_quit_button(GtkButton *button, gpointer user_data);

void correct(GtkButton *button, gpointer user_data);

void grayscale_display(GtkButton *button, gpointer user_data);
void gamma_display(GtkButton *button, gpointer user_data);
void threshold_display(GtkButton *button, gpointer user_data);
void rotate_display(GtkButton *button, gpointer user_data);

int all_man(char* path, double angle)
{
    int grayscale = surface_to_grayscale(path);
    if (grayscale)
    {
        int gaussian = gaussian_blur("gray_image.jpeg");
        if (gaussian)
        {
            int gamma = surface_to_gamma("gau_image.jpeg", 0.623);
            if (gamma)
            {
                int binary = filter_threshold("gam_image.jpeg");
                if (binary)
                {
                    int rot = rotate_by_angle("bin_image.jpeg", angle);
                    if (rot)
                    {
                        int extracted = grid_crop("rot_image.jpeg");
                        if (extracted)
                        {
                            int row_extracted = grid_row_extract("c_image.jpeg");
                            if (row_extracted)
                            {
                                int numbers_extracted = grid_numbers_full_extract();
                                if (numbers_extracted)
                                {
                                    resize_all();
                                    if (numbers_extracted)
                                    {
                                        int magical = magic_wand_all();
                                        if (magical)
                                        {
                                            int inverted = invert_black_white_all();
                                            if (inverted)
                                            {
                                                predictions = predict_all();
                                                array_to_file("grid_00", predictions);

                                                char *filename = "grid_00";
                                                int solved = Solve(filename);
						if (solved)
                                                	Result_construct("grid_00","grid_00.result");
						else
							Result_construct("grid_00", "grid_00");
						printf("Main: all processes done!\n");
                                                return EXIT_SUCCESS;
                                            }
                                            errx(EXIT_FAILURE, "Main: Color inversion failed");
                                            //return EXIT_FAILURE;
                                        }
                                        errx(EXIT_FAILURE, "Main: Magic Wand failed");
                                        //return EXIT_FAILURE;
                                    }
                                    errx(EXIT_FAILURE, "Main: Resize numbers extracted failed");
                                    //return EXIT_FAILURE;
                                }
                                errx(EXIT_FAILURE, "Main: Numbers extraction failed");
                                //return EXIT_FAILURE;
                            }
                            errx(EXIT_FAILURE, "Main: Row Extraction failed");
                            //return EXIT_FAILURE;
                        }
                        errx(EXIT_FAILURE, "Main: Extraction failed");
                        //return EXIT_FAILURE;
                    }
                    errx(EXIT_FAILURE, "Main: Rotation failed");
                    //return EXIT_FAILURE;
                }
                errx(EXIT_FAILURE, "Main: Binarization failed");
                //return EXIT_FAILURE;
            }
            errx(EXIT_FAILURE, "Main: Gamma failed");
            //return EXIT_FAILURE;
        }
        errx(EXIT_FAILURE, "Main: Gauss failed");
        //return EXIT_FAILURE;
    }
    errx(EXIT_FAILURE, "Main: Grayscale failed");
    //return EXIT_FAILURE;
}

int all_auto(char* path)
{
    int grayscale = surface_to_grayscale(path);
    if (grayscale)
    {
        int gaussian = gaussian_blur("gray_image.jpeg");
        if (gaussian)
        {
            int gamma = surface_to_gamma("gau_image.jpeg", 0.623);
            if (gamma)
            {
                int binary = filter_threshold("gam_image.jpeg");
                if (binary)
                {
                    int rot = auto_rotate("bin_image.jpeg");
                    if (rot)
                    {
                        int extracted = grid_crop("rot_image.jpeg");
                        if (extracted)
                        {
                            int row_extracted = grid_row_extract("c_image.jpeg");
                            if (row_extracted)
                            {
                                int numbers_extracted = grid_numbers_full_extract();
                                if (numbers_extracted)
                                {
                                    resize_all();
                                    if (numbers_extracted)
                                    {
                                        int magical = magic_wand_all();
                                        if (magical)
                                        {
                                            int inverted = invert_black_white_all();
                                            if (inverted)
                                            {
                                                predictions = predict_all();
                                                array_to_file("grid_00", predictions);

                                                char *filename = "grid_00";
                                                int solve = Solve(filename);
                        			if (solve)
							Result_construct("grid_00","grid_00.result");
						else
							Result_construct("grid_00", "grid_00");
                                                printf("Main: all processes done!\n");
                                                return EXIT_SUCCESS;
                                            } 
                                            return EXIT_FAILURE;
                                        }
                                        return EXIT_FAILURE;
                                    }
                                    return EXIT_FAILURE;
                                }
                                return EXIT_FAILURE;
                            }
                            return EXIT_FAILURE;
                        }
                        return EXIT_FAILURE;
                    }
                    return EXIT_FAILURE;
                }
                return EXIT_FAILURE;
            }
            return EXIT_FAILURE;
        }
        return EXIT_FAILURE;
    }
    return EXIT_FAILURE;
}
                                             
/*
int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc < 3)
        errx(EXIT_FAILURE, "Usage: image_file process args\n");

    if (strcmp(argv[2], "all-man") == 0)
    {
        if (all_man(argv[1], strtol(argv[3], NULL, 10)))
        {
            printf("Main: all processes done with no errors.");
            return EXIT_SUCCESS;
        }
        return EXIT_FAILURE;
    }

    if (strcmp(argv[2], "gray") == 0)
    {
        int grayscale = surface_to_grayscale(argv[1]);
        if (grayscale)
        {
            printf("Main: grayscale done with no errors.\n");
            return EXIT_SUCCESS;
        }
        return EXIT_FAILURE;
    }

    if (strcmp(argv[2], "gauss") == 0)
    {
        int gaussian = gaussian_blur(argv[1]);
        if (gaussian)
        {
            printf("Main: gauss blur done with no errors.\n");
            return EXIT_SUCCESS;
        }
        return EXIT_FAILURE;
    }

    if (strcmp(argv[2], "gamma") == 0)
    {
        int gamma = surface_to_gamma(argv[1], 0.623);
        if (gamma)
        {
            printf("Main: gamma increase done with no errors.\n");
            return EXIT_SUCCESS;
        }
        return EXIT_FAILURE;
    }

    if (strcmp(argv[2], "bin") == 0)
    {
        int binary = filter_threshold(argv[1]);
        if (binary)
        {
            printf("Main: binarization done with no errors.\n");
            return EXIT_SUCCESS;
        }
        return EXIT_FAILURE;
    }

    if (strcmp(argv[2], "rotation") == 0)
    {
        int rot = rotate_by_angle(argv[1], strtol(argv[3], NULL, 10));
        if (rot)
        {
            printf("Main: rotation done with no errors.\n");
            return EXIT_SUCCESS;
        }
        return EXIT_FAILURE;
    }

    if (strcmp(argv[2], "auto-rotation") == 0)
    {
        int auto_rot = auto_rotate(argv[1]);
        if (auto_rot + 1)
        {
            printf("Main: auto-rotation done with no errors.\n");
            return EXIT_SUCCESS;
        }
        return EXIT_FAILURE;
    }

    if (strcmp(argv[2], "all-no-rotate") == 0)
    {
        int grayscale = surface_to_grayscale(argv[1]);
        if (grayscale)
        {
            int gaussian = gaussian_blur("gray_image.jpeg");
            if (gaussian)
            {
                int gamma = surface_to_gamma("gau_image.jpeg", 0.623);
                if (gamma)
                {
                    int binary = filter_threshold("gam_image.jpeg");
                    if (binary)
                    {
                        printf("Main: processes done with no errors.\n");
                        return EXIT_SUCCESS;
                    }
                    return EXIT_FAILURE;
                }
                return EXIT_FAILURE;
            }
            return EXIT_FAILURE;
        }
        return EXIT_FAILURE;
    }

    if (strcmp(argv[2], "all-man-rotate") == 0)
    {
        int grayscale = surface_to_grayscale(argv[1]);
        if (grayscale)
        {
            int gaussian = gaussian_blur("gray_image.jpeg");
            if (gaussian)
            {
                int gamma = surface_to_gamma("gau_image.jpeg", 0.623);
                if (gamma)
                {
                    int binary = filter_threshold("gam_image.jpeg");
                    if (binary)
                    {
                        int rot = rotate_by_angle("bin_image.jpeg",
                            strtol(argv[3], NULL, 10));
                        if (rot)
                        {
                            printf("Main: processes done with no errors.\n");
                            return EXIT_SUCCESS;
                        }
                        return EXIT_FAILURE;
                    }
                    return EXIT_FAILURE;
                }
                return EXIT_FAILURE;
            }
            return EXIT_FAILURE;
        }
        return EXIT_FAILURE;
    }

    if (strcmp(argv[2], "all-auto-rotate") == 0)
    {
        int grayscale = surface_to_grayscale(argv[1]);
        if (grayscale)
        {
            int gaussian = gaussian_blur("gray_image.jpeg");
            if (gaussian)
            {
                int gamma = surface_to_gamma("gau_image.jpeg", 0.623);
                if (gamma)
                {
                    int binary = filter_threshold("gam_image.jpeg");
                    if (binary)
                    {
                        int auto_rot = auto_rotate("bin_image.jpeg");
                        if (auto_rot + 1)
                        {
                            printf("Main: processes done with no errors\n");
                            return EXIT_SUCCESS;
                        }
                        return EXIT_FAILURE;
                    }
                    return EXIT_FAILURE;
                }
                return EXIT_FAILURE;
            }
            return EXIT_FAILURE;
        }
        return EXIT_FAILURE;
    }

    if (strcmp(argv[2], "grid_extract") == 0)
    {
		int extracted = grid_crop("rot_image.jpeg");
        if (!extracted)
        {
            int binary = blackwhite("gray_image.jpeg");
            if (binary)
            {
                extracted = grid_crop("bin_image.jpeg");
            }
        }
		if (extracted)
		{
		    int row_extracted = grid_row_extract("c_image.jpeg");
		    if (row_extracted)
		    {
		        int numbers_extracted = grid_numbers_full_extract();
		        if (numbers_extracted)
		        {
		            printf("Main: processes done with no errors!\n");
                    resize_all();
		            return EXIT_SUCCESS;
		        }
		        return EXIT_FAILURE;
		    }
		    return EXIT_FAILURE;
		}
		return EXIT_FAILURE;
    }

    if (strcmp(argv[2], "magic-wand") == 0)
    {
        int magical = magic_wand(argv[1]);
        if (magical)
        {
            printf("Main: processes done with no errors!\n");
            return EXIT_SUCCESS;
        }
        return EXIT_FAILURE;
    }

    if (strcmp(argv[2], "all-magic-wand") == 0)
    {
        int magical = magic_wand_all();
        if (magical)
        {
            printf("Main: processes done with no errors!\n");
            return EXIT_SUCCESS;
        }
        return EXIT_FAILURE;
    }

    if (strcmp(argv[2], "all-invert") == 0)
    {
        int inverted = invert_black_white_all();
        if (inverted)
        {
            printf("Main: processes done with no errors!\n");
            return EXIT_SUCCESS;
        }
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
*/

int main(int argc, char **argv)
{
	gtk_init(&argc, &argv);
	
	GtkWidget *MainWindow = NULL;
	gchar *filename = NULL;
	GError *error = NULL;
	SGlobalData data;

	data.builder = gtk_builder_new();

	filename = g_build_filename("gui.glade", NULL);

	gtk_builder_add_from_file(data.builder, filename, &error);
	g_free(filename);

	gtk_builder_connect_signals(data.builder, &data);

	MainWindow = GTK_WIDGET(gtk_builder_get_object(data.builder, "MainWindow"));
	g_signal_connect(G_OBJECT(MainWindow), "delete-event", G_CALLBACK(gtk_main_quit), NULL);
	//g_signal_connect(G_OBJECT(MainWindow), "test", G_CALLBACK(on_toggle1_toggled), NULL);
	g_signal_connect(GTK_BUTTON(gtk_builder_get_object(data.builder, "Quit")), "clicked",  G_CALLBACK(gtk_main_quit), NULL);  

	gtk_widget_show_all(MainWindow);
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data.builder, "ImageSudoku")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data.builder, "SolveButton")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data.builder, "SaveButton")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data.builder, "RotateButton")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data.builder, "AngleIndication")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data.builder, "Scroller")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data.builder, "Quit")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data.builder, "Gamma")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data.builder, "Grayscale")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data.builder, "Threshold")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data.builder, "Rotate")));	
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data.builder, "Original")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data.builder, "Entry")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data.builder, "Correct")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data.builder, "Value")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data.builder, "Fixed_correct")));
	gtk_main();

	return EXIT_SUCCESS;
}


void callback_loadbutton(GtkButton *button, gpointer user_data)
{
	SGlobalData *data = (SGlobalData*)(user_data);
	GtkWidget *dialog;
     	dialog = gtk_file_chooser_dialog_new ("Select Sudoku Image", GTK_WINDOW(gtk_builder_get_object(data->builder, "MainWindow")), GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);

	
     	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
   	{
		//GtkWidget *fixed = GTK_WIDGET(user_data);

	 	// Move the button to a new position (100, 100)
 		//gtk_fixed_move(GTK_FIXED(gtk_builder_get_object(data->builder, "MainWindow")), GTK_WIDGET(gtk_builder_get_object(data->builder, "Quit")), 700, 700);
		
         	filename_img = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));


		GdkPixbuf* pixbuf=NULL;
 		pixbuf= gdk_pixbuf_new_from_file(filename_img, NULL);
		
		pixbuf = gdk_pixbuf_scale_simple(pixbuf, 700, 700, GDK_INTERP_BILINEAR);
		gtk_image_set_from_pixbuf((GtkImage*)gtk_builder_get_object(data->builder, "ImageSudoku"), pixbuf);

		gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(data->builder, "ImageSudoku")));
		gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(data->builder, "SolveButton")));
		gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data->builder, "SaveButton")));
		gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(data->builder, "AngleIndication")));
		gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(data->builder, "Scroller")));
	//	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(data->builder, "RotateButton")));
		gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(data->builder, "Quit")));

		gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(data->builder, "Gamma")));
		gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(data->builder, "Grayscale")));
		gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(data->builder, "Threshold")));
		gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(data->builder, "Rotate")));
		gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data->builder, "Original")));

		gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data->builder, "Entry")));
		gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data->builder, "Correct")));
		gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data->builder, "Value")));

		gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data->builder, "Fixed_correct")));
        	all_auto(filename_img);
	}

     	gtk_widget_destroy (dialog);
}

void callback_solve(GtkButton *button, gpointer user_data)
{
	SGlobalData *data = (SGlobalData*)(user_data);
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(data->builder, "SaveButton")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data->builder, "SolveButton")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data->builder, "ManualRotation")));
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(data->builder, "AngleIndication")));
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(data->builder, "Scroller")));
	//gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(data->builder, "RotateButton")));	
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(data->builder, "Quit")));
	
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(data->builder, "Value")));
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(data->builder, "Entry")));
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(data->builder, "Correct")));
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(data->builder, "Fixed_correct")));
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data->builder, "Original")));

	GtkScale *scale = GTK_SCALE(gtk_builder_get_object(data->builder, "Scroller"));
	double rotation_value = gtk_range_get_value(GTK_RANGE(scale));
	//g_print("%f \n", rotation_value);
	if (rotation_value!=0)
       		 all_man(filename_img, rotation_value);
    
	
    GdkPixbuf* pixbuf=NULL;
    pixbuf= gdk_pixbuf_new_from_file("solved.bmp", NULL);

    pixbuf = gdk_pixbuf_scale_simple(pixbuf, 700, 700, GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf((GtkImage*)gtk_builder_get_object(data->builder, "ImageSudoku"), pixbuf);
}

void callback_savebutton(GtkButton *button, gpointer user_data)
{
	SGlobalData *data = (SGlobalData*)(user_data);
	GtkWidget *dialog;
     	dialog = gtk_file_chooser_dialog_new ("Save Solution", GTK_WINDOW(gtk_builder_get_object(data->builder, "MainWindow")), GTK_FILE_CHOOSER_ACTION_SAVE, "_Cancel", GTK_RESPONSE_CANCEL, "_Save", GTK_RESPONSE_ACCEPT, NULL);
	gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);

    	 
	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		char *filename;

		//. ici j'ai fais des modifications pour commencer a save l'image
         	//filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		
		// declaring a path 
		const gchar *file_path = "./sudoku_result.png";

		// getting the image to save
		GtkImage *image = GTK_IMAGE(gtk_builder_get_object(data->builder,"ImageSudoku"));

		// get the pixbuf depuis le gtk image
		GdkPixbuf *pixbuf = gtk_image_get_pixbuf(image);

		//enfin on sauvegarde
		gdk_pixbuf_save(pixbuf,file_path, "png", NULL, NULL);

		// g_free(filename);
       	}
     	gtk_widget_destroy (dialog);

}

void correct(GtkButton *button, gpointer user_data)
{
	SGlobalData *data = (SGlobalData*)(user_data);
    const gchar* str  = gtk_entry_get_text (GTK_ENTRY (gtk_builder_get_object(data->builder, "CorrectValue")));
    int x=atoi(&str[0]);
    int y=atoi(&str[2]);
    int value=atoi(&str[4]);
    FILE *fp = fopen("grid_00", "r+");
    int i=0;
    int j=0;
    int c;
    if (value==0)
	    value=10;
    g_print("%i \n",value);
    g_print("%i \n",predictions[(x)*9+(y)]);
    predictions[(y)*9+(x)]=value;

    g_print("%i \n",predictions[(x)*9+(y)]);
  /*  while ((c = fgetc(fp)) != EOF ) {
    // Check if the character is the letter we want to replace
    if (c==' ')
        continue;
    if (c=='\n'){
        if (j==1)
            continue;
        i++;
        j=1;
        continue;
    }
    if (j==y && i==x ) {       
        // Move the file pointer back to the position of the character
        fseek(fp, -1, SEEK_CUR);
	
        // Replace the character with value
        fputc(value, fp);
        break;
    }
    j++;
    //printf("%i ,%i \n",(x-1)*9+y,j);
    }
    // Close the file
    fclose(fp);
*/
    
	array_to_file("grid_00.result", predictions);
	array_to_file("grid_00", predictions);
	Solve("grid_00");
	Result_construct("grid_00","grid_00.result");

    GdkPixbuf* pixbuf=NULL;
    pixbuf= gdk_pixbuf_new_from_file("solved.bmp", NULL);

    pixbuf = gdk_pixbuf_scale_simple(pixbuf, 700, 700, GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf((GtkImage*)gtk_builder_get_object(data->builder, "ImageSudoku"), pixbuf);
}

void original_display(GtkButton *button, gpointer user_data)
{
	SGlobalData *data = (SGlobalData*)(user_data);
	GdkPixbuf* pixbuf=NULL;
 	pixbuf= gdk_pixbuf_new_from_file(filename_img, NULL);

	pixbuf = gdk_pixbuf_scale_simple(pixbuf, 700, 700, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf((GtkImage*)gtk_builder_get_object(data->builder, "ImageSudoku"), pixbuf);
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data->builder, "SaveButton")));
}

void gamma_display(GtkButton *button, gpointer user_data)
{
	SGlobalData *data = (SGlobalData*)(user_data);
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(data->builder, "SolveButton")));
	GdkPixbuf* pixbuf=NULL;
 	pixbuf= gdk_pixbuf_new_from_file("gam_image.jpeg", NULL);
	
	pixbuf = gdk_pixbuf_scale_simple(pixbuf, 700, 700, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf((GtkImage*)gtk_builder_get_object(data->builder, "ImageSudoku"), pixbuf);
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data->builder, "SaveButton")));
}

void grayscale_display(GtkButton *button, gpointer user_data)
{
	SGlobalData *data = (SGlobalData*)(user_data);
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(data->builder, "SolveButton")));
	GdkPixbuf* pixbuf=NULL;
 	pixbuf= gdk_pixbuf_new_from_file("gray_image.jpeg", NULL);

	pixbuf = gdk_pixbuf_scale_simple(pixbuf, 700, 700, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf((GtkImage*)gtk_builder_get_object(data->builder, "ImageSudoku"), pixbuf);
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data->builder, "SaveButton")));
}

void threshold_display(GtkButton *button, gpointer user_data)
{
	SGlobalData *data = (SGlobalData*)(user_data);
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(data->builder, "SolveButton")));
	GdkPixbuf* pixbuf=NULL;
	pixbuf= gdk_pixbuf_new_from_file("bin_image.jpeg", NULL);

	pixbuf = gdk_pixbuf_scale_simple(pixbuf, 700, 700, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf((GtkImage*)gtk_builder_get_object(data->builder, "ImageSudoku"), pixbuf);
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data->builder, "SaveButton")));
}

void rotate_display(GtkButton *button, gpointer user_data)
{
	SGlobalData *data = (SGlobalData*)(user_data);
	gtk_widget_show(GTK_WIDGET(gtk_builder_get_object(data->builder, "SolveButton")));
	GdkPixbuf* pixbuf=NULL;
	pixbuf= gdk_pixbuf_new_from_file("rot_image.jpeg", NULL);

	pixbuf = gdk_pixbuf_scale_simple(pixbuf, 700, 700, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf((GtkImage*)gtk_builder_get_object(data->builder, "ImageSudoku"), pixbuf);
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(data->builder, "SaveButton")));
}

void solve_display(GtkButton *button, gpointer user_data)
{
	SGlobalData *data = (SGlobalData*)(user_data);
	GdkPixbuf* pixbuf=NULL;
	pixbuf= gdk_pixbuf_new_from_file("solved.bmp", NULL);

	pixbuf = gdk_pixbuf_scale_simple(pixbuf, 700, 700, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf((GtkImage*)gtk_builder_get_object(data->builder, "ImageSudoku"), pixbuf);
}


void on_quit_button(GtkButton *button, gpointer pointer)
{
	gtk_main_quit();
}
