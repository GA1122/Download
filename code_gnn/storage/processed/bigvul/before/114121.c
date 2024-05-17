on_forward_button_clicked(void *user_data, Evas_Object *forward_button, void *event_info)
{
    Browser_Window *app_data = (Browser_Window *)user_data;

    ewk_view_forward(app_data->webview);
     
    elm_object_disabled_set(forward_button, !ewk_view_forward_possible(app_data->webview));
}
