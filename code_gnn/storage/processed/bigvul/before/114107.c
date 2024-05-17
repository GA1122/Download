on_auth_ok(void *user_data, Evas_Object *obj, void *event_info)
{
    AuthData *auth_data = (AuthData *)user_data;

    const char *username = elm_entry_entry_get(auth_data->username_entry);
    const char *password = elm_entry_entry_get(auth_data->password_entry);
    ewk_auth_request_authenticate(auth_data->request, username, password);

    auth_popup_close(auth_data);
}
