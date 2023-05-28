#include <gtk/gtk.h>

#define MAX_BUF_LEN 3000
#define MAX_WIDTH_CHARS 80

static void destroy(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
}

int main(int argc, char *argv[])
{
    /* if (argc < 2) */
	  /* return 1; */

    /* int len = strlen(argv[1]); */
    /* if (argv[1][len-1] == '\n') */
    /*   argv[1][len-1] = '\0'; */
    char *text =
    "おも しろ・い [4]【面白い】\n\
（形）〔文〕ク おもしろ・し\n\
〔「面（おも）白し」で，目の前がぱっと明るくなる感じを表すのが原義といわれる〕\n\
①楽しい。愉快だ。「昨日見た映画は―・かった」「勉強が―・くて仕方がない」\n\
②興味をそそる。興味深い。「何か―・い話はないか」「最後にきて―・い展開を見せる」\n\
③こっけいだ。おかしい。「―・いしぐさで人を笑わせる」\n\
④（多く，打ち消しの語を伴う）心にかなう。好ましい。望ましい。「病状が―・くない」「―・くない結果に終わる」「私に―・からぬ感情を抱いている」\n\
⑤景色などが明るく広々とした感じで，気分がはればれとするようだ。明るく目が覚めるようだ。「十日あまりなれば，月―・し／土左日記」\n\
⑥心をひかれる。趣が深い。風流だ。「昔を思ひやりてみれば―・かりける所なり／土左日記」〔類義の語に「おかしい」があるが，「おかしい」は格好・表情・しぐさ・話し方などが普通と違っていて，笑いたくなる意を表す。それに対して「おもしろい」は対象が普通の基準から見ると新鮮・奇抜で変化に富んでいて，興味をそそる意を表す〕\n\
〔派生〕― が・る（動ラ五）・― げ（形動）・― さ（名）・― み（名） ";

    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Run");
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_set_keep_above(GTK_WINDOW(window), 1);
    g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);

    GtkWidget *label = gtk_label_new(text);
    gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
    gtk_label_set_max_width_chars(GTK_LABEL(label), MAX_WIDTH_CHARS);
    gtk_label_set_selectable(GTK_LABEL(label), TRUE);
    gtk_container_add(GTK_CONTAINER(window), label);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_MOUSE);
    gtk_widget_show_all(window);

    gtk_widget_set_sensitive(label, TRUE);
    gtk_widget_grab_focus(label);

    gtk_main();

    return 0;
}
