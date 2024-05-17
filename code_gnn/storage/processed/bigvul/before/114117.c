on_file_chooser_request(void *user_data, Evas_Object *webview, void *event_info)
{
    Browser_Window *app_data = (Browser_Window *)user_data;
    Ewk_File_Chooser_Request *request = (Ewk_File_Chooser_Request *)event_info;

    Evas_Object *window = elm_win_add(app_data->window, "file-picker-window", ELM_WIN_DIALOG_BASIC);
    elm_win_title_set(window, "File picker");
    elm_win_modal_set(window, EINA_TRUE);

    FileSelectorData* fs_data = (FileSelectorData*)malloc(sizeof(FileSelectorData));
    fs_data->parent = app_data->window;
    fs_data->window = window;
    fs_data->request = ewk_file_chooser_request_ref(request);
    evas_object_smart_callback_add(window, "delete,request", on_filepicker_deletion, fs_data);
    evas_object_event_callback_add(app_data->window, EVAS_CALLBACK_DEL, on_filepicker_parent_deletion, fs_data);

    Evas_Object *file_selector = elm_fileselector_add(window);
    const char *home_path = getenv("HOME");
    elm_fileselector_path_set(file_selector, home_path ? home_path : "/home");
    evas_object_size_hint_weight_set(file_selector, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_win_resize_object_add(window, file_selector);
    evas_object_show(file_selector);

    evas_object_smart_callback_add(file_selector, "done", on_fileselector_done, fs_data);

    evas_object_resize(window, 400, 400);
    elm_win_center(window, EINA_TRUE, EINA_TRUE);
    evas_object_show(window);
}
