static void event_rb_was_toggled(GtkButton *button, gpointer user_data)
{
     
    GList *found = g_list_find_custom(g_list_events, button, find_by_button);
    if (found)
    {
        event_gui_data_t *evdata = found->data;
        if (gtk_toggle_button_get_active(evdata->toggle_button))
        {
            free(g_event_selected);
            g_event_selected = xstrdup(evdata->event_name);
            check_event_config(evdata->event_name);

            clear_warnings();
            const bool good_rating = check_minimal_bt_rating(g_event_selected);
            show_warnings();

            gtk_widget_set_sensitive(g_btn_next, good_rating);
        }
    }
}