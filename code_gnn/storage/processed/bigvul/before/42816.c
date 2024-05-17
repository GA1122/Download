static char *get_next_processed_event(GList **events_list)
{
    if (!events_list || !*events_list)
        return NULL;

    char *event_name = (char *)(*events_list)->data;
    const size_t event_len = strlen(event_name);

     
    *events_list = g_list_delete_link(*events_list, *events_list);

    if (event_name[event_len - 1] == '*')
    {
        log_info("Expanding event '%s'", event_name);

        struct dump_dir *dd = dd_opendir(g_dump_dir_name, DD_OPEN_READONLY);
        if (!dd)
            error_msg_and_die("Can't open directory '%s'", g_dump_dir_name);

         
        event_name[event_len - 1] = '\0';

         
        char *expanded_events = list_possible_events(dd, g_dump_dir_name, event_name);

        dd_close(dd);
        free(event_name);

        GList *expanded_list = NULL;
         
        char *next = event_name = expanded_events;
        while ((next = strchr(event_name, '\n')))
        {
             
            next[0] = '\0';

             
            event_name = xstrdup(event_name);
            log_debug("Adding a new expanded event '%s' to the processed list", event_name);

             
            ++next;
            if (next[0] == '\0')
                break;

            expanded_list = g_list_prepend(expanded_list, event_name);

             
            event_name = next;
        }

        free(expanded_events);

         
        if (event_name != expanded_events)
             
            *events_list = g_list_concat(expanded_list, *events_list);
        else
        {
            log_info("No event was expanded, will continue with the next one.");
             
            return get_next_processed_event(events_list);
        }
    }

    clear_warnings();
    const bool acceptable = check_minimal_bt_rating(event_name);
    show_warnings();

    if (!acceptable)
    {
         
        free(event_name);

        g_list_free_full(*events_list, free);
        *events_list = NULL;
        return NULL;
    }

    return event_name;
}
