directory_load_done (NautilusDirectory *directory,
                     GError            *error)
{
    GList *node;

    nautilus_profile_start (NULL);
    g_object_ref (directory);

    directory->details->directory_loaded = TRUE;
    directory->details->directory_loaded_sent_notification = FALSE;

    if (error != NULL)
    {
         
        for (node = directory->details->file_list;
             node != NULL; node = node->next)
        {
            set_file_unconfirmed (NAUTILUS_FILE (node->data), FALSE);
        }

        nautilus_directory_emit_load_error (directory, error);
    }

     
    if (directory->details->dequeue_pending_idle_id != 0)
    {
        g_source_remove (directory->details->dequeue_pending_idle_id);
    }
    dequeue_pending_idle_callback (directory);

    directory_load_cancel (directory);

    g_object_unref (directory);
    nautilus_profile_end (NULL);
}
