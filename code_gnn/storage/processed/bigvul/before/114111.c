on_close_window(void *user_data, Evas_Object *webview, void *event_info)
{
    window_close((Browser_Window *)user_data);
}
