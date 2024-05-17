nautilus_directory_force_reload_internal (NautilusDirectory      *directory,
                                          NautilusFileAttributes  file_attributes)
{
    nautilus_profile_start (NULL);

     
    nautilus_directory_invalidate_file_attributes (directory, file_attributes);

     
    file_list_cancel (directory);
    directory->details->directory_loaded = FALSE;

     
    nautilus_directory_invalidate_count_and_mime_list (directory);

    add_all_files_to_work_queue (directory);
    nautilus_directory_async_state_changed (directory);

    nautilus_profile_end (NULL);
}
