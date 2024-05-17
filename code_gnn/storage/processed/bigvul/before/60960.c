nautilus_directory_async_state_changed (NautilusDirectory *directory)
{
     

    if (directory->details->in_async_service_loop)
    {
        directory->details->state_changed = TRUE;
        return;
    }
    directory->details->in_async_service_loop = TRUE;
    nautilus_directory_ref (directory);
    do
    {
        directory->details->state_changed = FALSE;
        start_or_stop_io (directory);
        if (call_ready_callbacks (directory))
        {
            directory->details->state_changed = TRUE;
        }
    }
    while (directory->details->state_changed);
    directory->details->in_async_service_loop = FALSE;
    nautilus_directory_unref (directory);

     
    async_job_wake_up ();
}
