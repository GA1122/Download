on_tooltip_text_unset(void *user_data, Evas_Object *obj, void *event_info)
{
    Browser_Window *window = (Browser_Window *)user_data;

    elm_object_tooltip_unset(window->webview);
}
