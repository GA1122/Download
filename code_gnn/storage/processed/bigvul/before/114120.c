on_fileselector_done(void *user_data, Evas_Object *file_selector, void *event_info)
{
    FileSelectorData *fs_data = (FileSelectorData *)user_data;

    const char *selected = (const char *)event_info;
    if (selected && *selected)
        ewk_file_chooser_request_file_choose(fs_data->request, selected);

    close_file_picker(fs_data);
}
