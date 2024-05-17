on_view_icon_changed(void *user_data, Evas_Object *webview, void *event_info)
{
    Browser_Window *app_data = (Browser_Window *)user_data;
     
    Ewk_Context *context = ewk_view_context_get(webview);
    Ewk_Favicon_Database *icon_database = ewk_context_favicon_database_get(context);

    const char *page_url = ewk_view_url_get(webview);
    Evas *evas = evas_object_evas_get(webview);
    ewk_favicon_database_async_icon_get(icon_database, page_url, evas, on_favicon_received, app_data);
}
