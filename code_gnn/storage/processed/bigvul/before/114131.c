on_refresh_button_clicked(void *user_data, Evas_Object *refresh_button, void *event_info)
{
    Browser_Window *app_data = (Browser_Window *)user_data;

    Evas *evas = evas_object_evas_get(refresh_button);
    Eina_Bool ctrlPressed = evas_key_modifier_is_set(evas_key_modifier_get(evas), "Control");
    if (ctrlPressed) {
        info("Reloading and bypassing cache...\n");
        ewk_view_reload_bypass_cache(app_data->webview);
    } else {
        info("Reloading...\n");
        ewk_view_reload(app_data->webview);
    }
}
