#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "function.h"

int main(int argc, char *argv[])

{

    GtkBuilder      *builder; 
    GtkWidget       *window;
    app_widgets     *widgets = g_slice_new(app_widgets);

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("glade/window_main.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));

    //get pointer to entry widgets ->entry connexion
    widgets->entry_link = GTK_WIDGET(gtk_builder_get_object(builder, "entry_link"));
    widgets->entry_nameFile = GTK_WIDGET(gtk_builder_get_object(builder, "entry_nameFile"));
    widgets->lbl_result = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_result"));

    gtk_builder_connect_signals(builder, widgets);

    g_object_unref(builder);

    gtk_widget_show(window);                
    gtk_main();
    g_slice_free(app_widgets, widgets);

    return 0;
}

void submit(GtkButton * button,app_widgets * submitFile){


    char *link = gtk_entry_get_text (GTK_ENTRY(submitFile->entry_link));
    char *nameFile = gtk_entry_get_text (GTK_ENTRY(submitFile->entry_nameFile));

    printf("link : %s\n",link);
    printf("nameFile : %s\n",nameFile);
    
    CURL *curl;
    FILE *fp;

    fp = fopen(nameFile,"wb");
    curl = curl_easy_init();

    curl_easy_setopt(curl,CURLOPT_URL,link);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
    curl_easy_setopt(curl,CURLOPT_FAILONERROR,1L);

    if(curl_easy_perform(curl) == CURLE_OK) {
        printf("download succes\n");
        gtk_label_set_text(GTK_LABEL(submitFile->lbl_result),"dowload succes");
    }
    else{
        printf("ERROR: %s\n",curl_easy_strerror(curl_easy_perform(curl)));
        gtk_label_set_text(GTK_LABEL(submitFile->lbl_result),curl_easy_strerror(curl_easy_perform(curl)));
    }
    fclose(fp);

    curl_easy_cleanup(curl);    

}
// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}
