on_javascript_prompt(Ewk_View_Smart_Data *smartData, const char *message, const char *default_value)
{
    Browser_Window *window = browser_view_find(smartData->self);

    Eina_Bool ok = EINA_FALSE;

    Evas_Object *prompt_popup = elm_popup_add(window->window);
    evas_object_size_hint_weight_set(prompt_popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_object_part_text_set(prompt_popup, "title,text", "Prompt");

     
    Evas_Object *box = elm_box_add(window->window);
    elm_box_padding_set(box, 0, 4);
    evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_show(box);

    Evas_Object *prompt = elm_label_add(window->window);
    elm_object_text_set(prompt, message);
    evas_object_size_hint_weight_set(prompt, EVAS_HINT_EXPAND, 0.0);
    evas_object_size_hint_align_set(prompt, EVAS_HINT_FILL, 0.5);
    elm_box_pack_end(box, prompt);
    evas_object_show(prompt);

    Evas_Object *entry = elm_entry_add(window->window);
    elm_entry_scrollable_set(entry, EINA_TRUE);
    elm_entry_single_line_set(entry, EINA_TRUE);
    elm_entry_text_style_user_push(entry, "DEFAULT='font_size=18'");
    elm_entry_entry_set(entry, default_value ? default_value : "");
    elm_entry_select_all(entry);
    evas_object_size_hint_weight_set(entry, EVAS_HINT_EXPAND, 0.0);
    evas_object_size_hint_align_set(entry, EVAS_HINT_FILL, 0.5);
    elm_box_pack_end(box, entry);
    elm_object_focus_set(entry, EINA_TRUE);
    evas_object_show(entry);

    elm_object_content_set(prompt_popup, box);

     
    Evas_Object *cancel_button = elm_button_add(prompt_popup);
    elm_object_text_set(cancel_button, "Cancel");
    elm_object_part_content_set(prompt_popup, "button1", cancel_button);
    evas_object_smart_callback_add(cancel_button, "clicked", quit_event_loop, NULL);
    Evas_Object *ok_button = elm_button_add(prompt_popup);
    elm_object_text_set(ok_button, "OK");
    elm_object_part_content_set(prompt_popup, "button2", ok_button);
    evas_object_smart_callback_add(ok_button, "clicked", on_ok_clicked, &ok);
    evas_object_show(prompt_popup);

     
    ecore_main_loop_begin();

     
    const char *prompt_text = ok ? eina_stringshare_add(elm_entry_entry_get(entry)) : NULL;
    evas_object_del(prompt_popup);

    return prompt_text;
}
