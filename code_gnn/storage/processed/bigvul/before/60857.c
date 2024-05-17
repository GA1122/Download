call_ready_callbacks_at_idle (gpointer callback_data)
{
    NautilusDirectory *directory;
    GList *node, *next;
    ReadyCallback *callback;

    directory = NAUTILUS_DIRECTORY (callback_data);
    directory->details->call_ready_idle_id = 0;

    nautilus_directory_ref (directory);

    callback = NULL;
    while (1)
    {
         
        for (node = directory->details->call_when_ready_list;
             node != NULL; node = next)
        {
            next = node->next;
            callback = node->data;
            if (!callback->active)
            {
                 
                break;
            }
        }
        if (node == NULL)
        {
            break;
        }

         
        remove_callback_link_keep_data (directory, node);

         
        ready_callback_call (directory, callback);
        g_free (callback);
    }

    nautilus_directory_async_state_changed (directory);

    nautilus_directory_unref (directory);

    return FALSE;
}
