on_url_bar_clicked(void *user_data, Evas_Object *url_bar, void *event_info)
{
    Browser_Window *app_data = (Browser_Window *)user_data;

     
    evas_object_focus_set(app_data->webview, EINA_FALSE);
    elm_object_focus_set(url_bar, EINA_TRUE);
}
