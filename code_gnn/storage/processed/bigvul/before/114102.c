static Browser_Window *browser_window_find(Evas_Object *window)
{
    Eina_List *l;
    void *data;

    if (!window)
        return NULL;

    EINA_LIST_FOREACH(windows, l, data) {
        Browser_Window *browser_window = (Browser_Window *)data;
        if (browser_window->window == window)
            return browser_window;
    }
    return NULL;
}
