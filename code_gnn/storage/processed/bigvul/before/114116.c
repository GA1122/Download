on_favicon_received(const char *page_url, Evas_Object *icon, void *event_info)
{
    Browser_Window *app_data = (Browser_Window *)event_info;
    if (strcmp(page_url, ewk_view_url_get(app_data->webview)))
        return;

     
    Evas_Object *old_icon = elm_object_part_content_unset(app_data->url_bar, "icon");
    if (old_icon) {
        evas_object_unref(old_icon);
        evas_object_del(old_icon);
    }

     
    if (icon) {
         
        evas_object_size_hint_min_set(icon, 48, 24);
        evas_object_image_filled_set(icon, EINA_FALSE);
        evas_object_image_fill_set(icon, 24, 0, 24, 24);
        elm_object_part_content_set(app_data->url_bar, "icon", icon);
        evas_object_ref(icon);
    }
}
