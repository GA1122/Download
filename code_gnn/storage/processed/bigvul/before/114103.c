static void close_file_picker(FileSelectorData* fs_data)
{
    evas_object_event_callback_del(fs_data->parent, EVAS_CALLBACK_DEL, on_filepicker_parent_deletion);
    evas_object_del(fs_data->window);
    ewk_file_chooser_request_unref(fs_data->request);
    free(fs_data);
}
