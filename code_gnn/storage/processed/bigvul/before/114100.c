auth_popup_close(AuthData *auth_data)
{
    ewk_auth_request_unref(auth_data->request);
    evas_object_del(auth_data->popup);
    free(auth_data);
}
