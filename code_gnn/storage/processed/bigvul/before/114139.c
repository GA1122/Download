on_window_deletion(void *user_data, Evas_Object *window, void *event_info)
{
    window_close(browser_window_find(window));
}
