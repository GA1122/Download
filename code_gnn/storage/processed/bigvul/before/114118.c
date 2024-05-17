on_filepicker_deletion(void *user_data, Evas_Object *window, void *event_info)
{
    close_file_picker((FileSelectorData *)user_data);
}
