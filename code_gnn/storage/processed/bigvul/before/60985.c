ready_callback_key_compare (gconstpointer a,
                            gconstpointer b)
{
    const ReadyCallback *callback_a, *callback_b;

    callback_a = a;
    callback_b = b;

    if (callback_a->file < callback_b->file)
    {
        return -1;
    }
    if (callback_a->file > callback_b->file)
    {
        return 1;
    }
    if (callback_a->file == NULL)
    {
         
        if ((void *) callback_a->callback.directory < (void *) callback_b->callback.directory)
        {
            return -1;
        }
        if ((void *) callback_a->callback.directory > (void *) callback_b->callback.directory)
        {
            return 1;
        }
    }
    else
    {
        if ((void *) callback_a->callback.file < (void *) callback_b->callback.file)
        {
            return -1;
        }
        if ((void *) callback_a->callback.file > (void *) callback_b->callback.file)
        {
            return 1;
        }
    }
    if (callback_a->callback_data < callback_b->callback_data)
    {
        return -1;
    }
    if (callback_a->callback_data > callback_b->callback_data)
    {
        return 1;
    }
    return 0;
}
