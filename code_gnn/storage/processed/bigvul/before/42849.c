static void on_next_btn_cb(GtkWidget *btn, gpointer user_data)
{
    gint current_page_no = gtk_notebook_get_current_page(g_assistant);
    gint next_page_no = select_next_page_no(current_page_no, NULL);

     
    if (current_page_no == next_page_no)
        on_page_prepare(g_assistant, gtk_notebook_get_nth_page(g_assistant, next_page_no), NULL);
    else
        gtk_notebook_set_current_page(g_assistant, next_page_no);
}