static char *setup_next_processed_event(GList **events_list)
{
    free(g_event_selected);
    g_event_selected = NULL;

    char *event = get_next_processed_event(&g_auto_event_list);
    if (!event)
    {
         
        gtk_label_set_text(g_lbl_event_log, _("Processing finished."));
         
        gtk_widget_hide(GTK_WIDGET(g_spinner_event_log));
        hide_next_step_button();
        return NULL;
    }

    log_notice("selected -e EVENT:%s", event);
    return event;
}