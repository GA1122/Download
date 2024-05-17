on_filepicker_parent_deletion(void *user_data, Evas *evas, Evas_Object *window, void *event)
{
    close_file_picker((FileSelectorData *)user_data);
}
