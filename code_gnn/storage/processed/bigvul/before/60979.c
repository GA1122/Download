new_files_callback (GObject      *source_object,
                    GAsyncResult *res,
                    gpointer      user_data)
{
    NautilusDirectory *directory;
    GFileInfo *info;
    NewFilesState *state;

    state = user_data;

    if (state->directory == NULL)
    {
         
        new_files_state_unref (state);
        return;
    }

    directory = nautilus_directory_ref (state->directory);

     
    info = g_file_query_info_finish (G_FILE (source_object), res, NULL);
    if (info != NULL)
    {
        directory_load_one (directory, info);
        g_object_unref (info);
    }

    new_files_state_unref (state);

    nautilus_directory_unref (directory);
}
