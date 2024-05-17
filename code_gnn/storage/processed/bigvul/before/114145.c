static void window_free(Browser_Window *window)
{
    evas_object_del(window->webview);
     
    evas_object_del(window->window);
    free(window);
}
