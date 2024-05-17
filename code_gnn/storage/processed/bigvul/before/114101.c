static Browser_Window *browser_view_find(Evas_Object *view)
{
    Eina_List *l;
    void *data;

    if (!view)
        return NULL;

    EINA_LIST_FOREACH(windows, l, data) {
        Browser_Window *browser_window = (Browser_Window *)data;
        if (browser_window->webview == view)
            return browser_window;
    }
    return NULL;
}
