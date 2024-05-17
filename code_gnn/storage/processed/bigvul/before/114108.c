on_authentication_request(void *user_data, Evas_Object *obj, void *event_info)
{
    Browser_Window *window = (Browser_Window *)user_data;
    Ewk_Auth_Request *request = ewk_auth_request_ref((Ewk_Auth_Request *)event_info);

    AuthData *auth_data = (AuthData *)malloc(sizeof(AuthData));
    auth_data->request = request;

    Evas_Object *auth_popup = elm_popup_add(window->window);
    auth_data->popup = auth_popup;
    evas_object_size_hint_weight_set(auth_popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_object_part_text_set(auth_popup, "title,text", "Authentication Required");

     
    Evas_Object *vbox = elm_box_add(auth_popup);
    elm_box_padding_set(vbox, 0, 4);
    evas_object_size_hint_weight_set(vbox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(vbox, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_object_content_set(auth_popup, vbox);
    evas_object_show(vbox);

     
    Evas_Object *label = elm_label_add(auth_popup);
    elm_label_line_wrap_set(label, ELM_WRAP_WORD);
    Eina_Strbuf *auth_text = eina_strbuf_new();
    const char* host = ewk_auth_request_host_get(request);
    const char* realm = ewk_auth_request_realm_get(request);
    eina_strbuf_append_printf(auth_text, "A username and password are being requested by %s. The site says: \"%s\"", host, realm ? realm : "");
    elm_object_text_set(label, eina_strbuf_string_get(auth_text));
    eina_strbuf_free(auth_text);
    evas_object_size_hint_weight_set(label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(label, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_box_pack_end(vbox, label);
    evas_object_show(label);

     
    Evas_Object *table = elm_table_add(auth_popup);
    elm_table_padding_set(table, 2, 2);
    elm_table_homogeneous_set(table, EINA_TRUE);
    evas_object_size_hint_weight_set(table, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(table, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_box_pack_end(vbox, table);
    evas_object_show(table);

     
    Evas_Object *username_label = elm_label_add(auth_popup);
    elm_object_text_set(username_label, "Username:");
    evas_object_size_hint_weight_set(username_label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(username_label, 1, EVAS_HINT_FILL);
    elm_table_pack(table, username_label, 0, 0, 1, 1);
    evas_object_show(username_label);

    Evas_Object *username_entry = elm_entry_add(auth_popup);
    auth_data->username_entry = username_entry;
    elm_entry_scrollable_set(username_entry, EINA_TRUE);
    elm_entry_single_line_set(username_entry, EINA_TRUE);
    elm_entry_text_style_user_push(username_entry, "DEFAULT='font_size=18'");
    const char *suggested_username = ewk_auth_request_suggested_username_get(request);
    elm_entry_entry_set(username_entry, suggested_username ? suggested_username : "");
    evas_object_size_hint_weight_set(username_entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(username_entry, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_table_pack(table, username_entry, 1, 0, 2, 1);
    elm_object_focus_set(username_entry, EINA_TRUE);
    evas_object_show(username_entry);

     
    Evas_Object *password_label = elm_label_add(auth_popup);
    elm_object_text_set(password_label, "Password:");
    evas_object_size_hint_weight_set(password_label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(password_label, 1, EVAS_HINT_FILL);
    elm_table_pack(table, password_label, 0, 1, 1, 1);
    evas_object_show(password_label);

    Evas_Object *password_entry = elm_entry_add(auth_popup);
    auth_data->password_entry = password_entry;
    elm_entry_scrollable_set(password_entry, EINA_TRUE);
    elm_entry_single_line_set(password_entry, EINA_TRUE);
    elm_entry_password_set(password_entry, EINA_TRUE);
    elm_entry_text_style_user_push(password_entry, "DEFAULT='font_size=18'");
    evas_object_size_hint_weight_set(password_entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(password_entry, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_table_pack(table, password_entry, 1, 1, 2, 1);
    evas_object_show(password_entry);

     
    Evas_Object *cancel_button = elm_button_add(auth_popup);
    elm_object_text_set(cancel_button, "Cancel");
    elm_object_part_content_set(auth_popup, "button1", cancel_button);
    evas_object_smart_callback_add(cancel_button, "clicked", on_auth_cancel, auth_data);
    Evas_Object *ok_button = elm_button_add(auth_popup);
    elm_object_text_set(ok_button, "OK");
    elm_object_part_content_set(auth_popup, "button2", ok_button);
    evas_object_smart_callback_add(ok_button, "clicked", on_auth_ok, auth_data);
    evas_object_show(auth_popup);
}
