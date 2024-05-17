static void update_event_log_on_disk(const char *str)
{
     
    struct dump_dir *dd = dd_opendir(g_dump_dir_name, 0);
    if (!dd)
        return;
    char *event_log = dd_load_text_ext(dd, FILENAME_EVENT_LOG, DD_FAIL_QUIETLY_ENOENT);

     
    unsigned len = strlen(event_log);
    if (len != 0 && event_log[len - 1] != '\n')
        event_log = append_to_malloced_string(event_log, "\n");
    event_log = append_to_malloced_string(event_log, str);

     
    len = strlen(event_log);
    char *new_log = event_log;
    if (len > EVENT_LOG_HIGH_WATERMARK)
    {
        new_log += len - EVENT_LOG_LOW_WATERMARK;
        new_log = strchrnul(new_log, '\n');
        if (new_log[0])
            new_log++;
    }

     
    dd_save_text(dd, FILENAME_EVENT_LOG, new_log);
    free(event_log);
    dd_close(dd);
}
