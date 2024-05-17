static void update_window_title(void)
{
     
    const char *prgname = g_get_prgname();
    const char *reason = problem_data_get_content_or_NULL(g_cd, FILENAME_REASON);
    char *title = xasprintf("%s - %s", (reason ? reason : g_dump_dir_name),
            (prgname ? prgname : "report"));
    gtk_window_set_title(g_wnd_assistant, title);
    free(title);
}