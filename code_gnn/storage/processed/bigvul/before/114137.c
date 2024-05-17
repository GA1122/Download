on_url_changed(void *user_data, Evas_Object *webview, void *event_info)
{
    Browser_Window *window = (Browser_Window *)user_data;
    elm_entry_entry_set(window->url_bar, ewk_view_url_get(window->webview));
}
