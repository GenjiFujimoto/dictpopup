#include <gtk/gtk.h>

#define BORDER_WIDTH 2
#define MARGIN 5

static void destroy(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
}

static gboolean on_button_press(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
    if (event->button == GDK_BUTTON_SECONDARY) {
	GtkWidget *window = gtk_widget_get_toplevel(widget);
        gtk_widget_destroy(window);
        return TRUE;
    }
    return FALSE;
}

int main(int argc, char *argv[])
{    
    if (argc < 2)
      return 1;
    char *text = argv[1];

    /* Temporary fix for trailing newline */
    int len = strlen(text);
    if (len && text[len - 1] == '\n') 
      text[len - 1] = '\0';

    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_POPUP);
    gtk_window_set_title(GTK_WINDOW(window), "dictpopup");
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_set_keep_above(GTK_WINDOW(window), 1);
    gtk_container_set_border_width(GTK_CONTAINER(window), BORDER_WIDTH);
    g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);
    
    GtkTextBuffer *textbuffer = gtk_text_buffer_new(NULL);
    gtk_text_buffer_set_text(textbuffer, text, -1);
    
    GtkWidget *textview = gtk_text_view_new_with_buffer(textbuffer);

    gtk_text_view_set_top_margin(GTK_TEXT_VIEW(textview), MARGIN);
    gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(textview), MARGIN);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(textview), MARGIN);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(textview), MARGIN);

    gtk_text_view_set_editable(GTK_TEXT_VIEW(textview), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textview), GTK_WRAP_CHAR);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textview), FALSE);
    gtk_widget_set_size_request(textview, 480, -1);
    gtk_container_add(GTK_CONTAINER(window), textview);

    g_signal_connect(textview, "button-press-event", G_CALLBACK(on_button_press), NULL);
    
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_MOUSE);
    gtk_widget_show_all(window);

    gtk_main();
    
    return 0;
}
