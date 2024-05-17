on_auth_cancel(void *user_data, Evas_Object *obj, void *event_info)
{
    AuthData *auth_data = (AuthData *)user_data;

    ewk_auth_request_cancel(auth_data->request);

    auth_popup_close(auth_data);
}
