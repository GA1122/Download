count_children_done (NautilusDirectory *directory,
                     NautilusFile      *count_file,
                     gboolean           succeeded,
                     int                count)
{
    g_assert (NAUTILUS_IS_FILE (count_file));

    count_file->details->directory_count_is_up_to_date = TRUE;

     
    if (!succeeded)
    {
        count_file->details->directory_count_failed = TRUE;
        count_file->details->got_directory_count = FALSE;
        count_file->details->directory_count = 0;
    }
    else
    {
        count_file->details->directory_count_failed = FALSE;
        count_file->details->got_directory_count = TRUE;
        count_file->details->directory_count = count;
    }
    directory->details->count_in_progress = NULL;

     
    nautilus_file_changed (count_file);

     
    async_job_end (directory, "directory count");
    nautilus_directory_async_state_changed (directory);
}
