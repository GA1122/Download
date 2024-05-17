on_progress(void *user_data, Evas_Object *webview, void *event_info)
{
    Browser_Window *window = (Browser_Window *)user_data;
    double progress = *(double *)event_info;

    title_set(window->window, ewk_view_title_get(window->webview), progress * 100);
}
