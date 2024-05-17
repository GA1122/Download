on_url_bar_activated(void *user_data, Evas_Object *url_bar, void *event_info)
{
    Browser_Window *app_data = (Browser_Window *)user_data;

    const char *user_url = elm_entry_entry_get(url_bar);
    char *url = url_from_user_input(user_url);
    ewk_view_url_set(app_data->webview, url);
    free(url);

     
    view_focus_set(app_data, EINA_TRUE);
}
