on_tooltip_text_set(void *user_data, Evas_Object *obj, void *event_info)
{
    Browser_Window *window = (Browser_Window *)user_data;
    const char* message = (const char*)event_info;

    elm_object_tooltip_text_set(window->webview, message);
    elm_object_tooltip_show(window->webview);
}
