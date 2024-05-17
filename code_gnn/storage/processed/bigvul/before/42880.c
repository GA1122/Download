static void toggle_eb_comment(void)
{
     
    if (pages[PAGENO_EDIT_COMMENT].page_widget == NULL)
        return;

    bool good =
        gtk_text_buffer_get_char_count(gtk_text_view_get_buffer(g_tv_comment)) >= 10
        || gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(g_cb_no_comment));

     
    gtk_widget_set_sensitive(g_btn_next, good);

     
    if (good)
        gtk_widget_hide(GTK_WIDGET(g_eb_comment));
    else
        gtk_widget_show(GTK_WIDGET(g_eb_comment));
}
