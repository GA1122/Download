on_new_window(void *user_data, Evas_Object *webview, void *event_info)
{
    Evas_Object **new_view = (Evas_Object **)event_info;
    Browser_Window *window = window_create(NULL);
    *new_view = window->webview;
    windows = eina_list_append(windows, window);
}
