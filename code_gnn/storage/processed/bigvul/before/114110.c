on_back_forward_list_changed(void *user_data, Evas_Object *webview, void *event_info)
{
    Browser_Window *window = (Browser_Window *)user_data;

     
    elm_object_disabled_set(window->back_button, !ewk_view_back_possible(webview));
    elm_object_disabled_set(window->forward_button, !ewk_view_forward_possible(webview));
}
