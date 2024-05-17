on_title_changed(void *user_data, Evas_Object *webview, void *event_info)
{
    Browser_Window *window = (Browser_Window *)user_data;
    const char *title = (const char *)event_info;

    title_set(window->window, title, 100);
}
