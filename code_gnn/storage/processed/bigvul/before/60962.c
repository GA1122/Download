nautilus_directory_cancel (NautilusDirectory *directory)
{
     
    deep_count_cancel (directory);
    directory_count_cancel (directory);
    file_info_cancel (directory);
    file_list_cancel (directory);
    link_info_cancel (directory);
    mime_list_cancel (directory);
    new_files_cancel (directory);
    extension_info_cancel (directory);
    thumbnail_cancel (directory);
    mount_cancel (directory);
    filesystem_info_cancel (directory);

     
    if (waiting_directories != NULL)
    {
        g_hash_table_remove (waiting_directories, directory);
    }

     
    async_job_wake_up ();
}
