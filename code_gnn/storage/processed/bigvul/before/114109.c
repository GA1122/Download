on_back_button_clicked(void *user_data, Evas_Object *back_button, void *event_info)
{
    Browser_Window *app_data = (Browser_Window *)user_data;

    ewk_view_back(app_data->webview);
     
    elm_object_disabled_set(back_button, !ewk_view_back_possible(app_data->webview));
}
