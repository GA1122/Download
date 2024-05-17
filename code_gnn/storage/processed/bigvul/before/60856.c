call_ready_callbacks (NautilusDirectory *directory)
{
    gboolean found_any;
    GList *node, *next;
    ReadyCallback *callback;

    found_any = FALSE;

     
    for (node = directory->details->call_when_ready_list;
         node != NULL; node = next)
    {
        next = node->next;
        callback = node->data;
        if (callback->active &&
            request_is_satisfied (directory, callback->file, callback->request))
        {
            callback->active = FALSE;
            found_any = TRUE;
        }
    }

    if (found_any)
    {
        schedule_call_ready_callbacks (directory);
    }

    return found_any;
}
