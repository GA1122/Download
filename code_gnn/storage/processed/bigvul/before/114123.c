on_javascript_alert(Ewk_View_Smart_Data *smartData, const char *message)
{
    Browser_Window *window = browser_view_find(smartData->self);

    Evas_Object *alert_popup = elm_popup_add(window->window);
    evas_object_size_hint_weight_set(alert_popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_object_text_set(alert_popup, message);
    elm_object_part_text_set(alert_popup, "title,text", "Alert");

     
    Evas_Object *button = elm_button_add(alert_popup);
    elm_object_text_set(button, "OK");
    elm_object_part_content_set(alert_popup, "button1", button);
    evas_object_smart_callback_add(button, "clicked", quit_event_loop, NULL);
    elm_object_focus_set(button, EINA_TRUE);
    evas_object_show(alert_popup);

     
    ecore_main_loop_begin();

    evas_object_del(alert_popup);
}
