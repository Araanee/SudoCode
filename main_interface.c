#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <err.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../utils/auxiliary.h"
#include "../pretreatment/rotation.h"
#include "../pretreatment/auto_rotation.h"
#include "../pretreatment/delete_color.h"
#include "../pretreatment/grayscale.h"
#include "../pretreatment/gaussian.h"
#include "../pretreatment/gamma.h"
#include "../pretreatment/threshold.h"
#include "../pretreatment/resize.h"
#include "../pretreatment/sobel.h"
#include "../grid/grid_detection.h"
#include "../grid/grid_row_extraction.h"
#include "../grid/grid_numbers_extraction.h"
#include "../main.h"
#include "../reconstruction/reconstruct.h"
#include "../resolution/resolution.h"
#include "../resolution/import_export.h"
#include "../grid_to_image/construct.h"
#include "../grid_to_image/pixel_operations.h"
#include "../grid_to_image/newreader.h"
#include "../ocr_neural_network/predicting.h"
#include "../ocr_neural_network/Neural.h"
#include "../ocr_neural_network/DataLoad.h"
#include "../ocr_neural_network/Persist.h"
#include "../ocr_neural_network/MathTools.h"


typedef struct
{
	GtkBuilder *builder;
      	gpointer user_data;
} SGlobalData;

char *filename_img;
void callback_solve(GtkButton *button, gpointer user_data);
void callback_loadbutton(GtkButton *button, gpointer user_data);
void callback_savebutton(GtkButton *button, gpointer user_data);
//void on_toggle1_toggled(GtkToggleButton *b,GtkBuilder builder);  
void rotate_img(GtkButton *button, gpointer user_data);
void on_quit_button(GtkButton *button, gpointer user_data);

void grayscale_display(GtkButton *button, gpointer user_data);
void gamma_display(GtkButton *button, gpointer user_data);
void threshold_display(GtkButton *button, gpointer user_data);
void rotate_display(GtkButton *button, gpointer user_data);

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
         	char *filename;
		
         	filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

         	//char *filename;

		GdkPixbuf* pixbuf=NULL;
 		pixbuf= gdk_pixbuf_new_from_file(filename, NULL);
		
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
		g_free(filename);
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
	
	GtkScale *scale = GTK_SCALE(gtk_builder_get_object(data->builder, "Scroller"));
	double rotation_value = gtk_range_get_value(GTK_RANGE(scale));
	//g_print("%f \n", rotation_value);
	if (rotation_value==0)
	{	all_man(filename_img, rotation_value);
	}
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

void gamma_display(GtkButton *button, gpointer user_data)
{
	SGlobalData *data = (SGlobalData*)(user_data);
	GdkPixbuf* pixbuf=NULL;
 	pixbuf= gdk_pixbuf_new_from_file("../gam_image.jpeg", NULL);
	
	pixbuf = gdk_pixbuf_scale_simple(pixbuf, 700, 700, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf((GtkImage*)gtk_builder_get_object(data->builder, "ImageSudoku"), pixbuf);
}

void grayscale_display(GtkButton *button, gpointer user_data)
{
	SGlobalData *data = (SGlobalData*)(user_data);
	GdkPixbuf* pixbuf=NULL;
 	pixbuf= gdk_pixbuf_new_from_file("../gray_image.jpeg", NULL);

	pixbuf = gdk_pixbuf_scale_simple(pixbuf, 700, 700, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf((GtkImage*)gtk_builder_get_object(data->builder, "ImageSudoku"), pixbuf);
}

void threshold_display(GtkButton *button, gpointer user_data)
{
	SGlobalData *data = (SGlobalData*)(user_data);
	GdkPixbuf* pixbuf=NULL;
	pixbuf= gdk_pixbuf_new_from_file("../bin_image.jpeg", NULL);

	pixbuf = gdk_pixbuf_scale_simple(pixbuf, 700, 700, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf((GtkImage*)gtk_builder_get_object(data->builder, "ImageSudoku"), pixbuf);
}

void rotate_display(GtkButton *button, gpointer user_data)
{
	SGlobalData *data = (SGlobalData*)(user_data);
	GdkPixbuf* pixbuf=NULL;
	pixbuf= gdk_pixbuf_new_from_file("../rot_image.jpeg", NULL);

	pixbuf = gdk_pixbuf_scale_simple(pixbuf, 700, 700, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf((GtkImage*)gtk_builder_get_object(data->builder, "ImageSudoku"), pixbuf);
}

void solve_display(GtkButton *button, gpointer user_data)
{
	SGlobalData *data = (SGlobalData*)(user_data);
	GdkPixbuf* pixbuf=NULL;
	pixbuf= gdk_pixbuf_new_from_file("../c_image.jpeg", NULL);

	pixbuf = gdk_pixbuf_scale_simple(pixbuf, 700, 700, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf((GtkImage*)gtk_builder_get_object(data->builder, "ImageSudoku"), pixbuf);
}

/*void rotate_img(GtkButton *button, gpointer user_data){
	
	SGlobalData *data = (SGlobalData*)(user_data);
	GtkButton* rotate_button = GTK_BUTTON(gtk_builder_get_object(data->builder, "RotateButton"));
	GtkScale *scale = GTK_SCALE(gtk_builder_get_object(data->builder, "scale_rotation"));
    	double rotation_value = gtk_range_get_value(GTK_RANGE(scale));
	
	if (rotation_value==0)
	{
        int grayscale = surface_to_grayscale(filename_img);
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
                            g_print("Auto_rotate: processes done with no errors\n");
                        }
                    }
                }
            }
        }
	
	}
	else
	{
	int grayscale = surface_to_grayscale(filename_img);
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
                        int rot = rotate_by_angle("bin_image.jpeg",rotation_value);
                        if (rot)
                        {
                           g_print("Rotate: processes done with no errors.\n");
                        }
                    }
                }
            }
        }

	}
	
	GdkPixbuf* pixbuf=NULL;
	pixbuf= gdk_pixbuf_new_from_file("../pretreatment/rot_image.jpeg", NULL);
	                                                                
	pixbuf = gdk_pixbuf_scale_simple(pixbuf, 700, 700, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf(gtk_builder_get_object(data->builder, "ImageSudoku"), pixbuf);

}*/

void on_quit_button(GtkButton *button, gpointer pointer)
{
	gtk_main_quit();
}
