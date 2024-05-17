on_home_button_clicked(void *user_data, Evas_Object *home_button, void *event_info)
{
    Browser_Window *app_data = (Browser_Window *)user_data;

    ewk_view_url_set(app_data->webview, DEFAULT_URL);
}
