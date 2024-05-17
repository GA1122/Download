view_focus_set(Browser_Window *window, Eina_Bool focus)
{
     
    elm_object_focus_set(elm_object_top_widget_get(window->window), EINA_FALSE);
    evas_object_focus_set(window->webview, focus);
}
