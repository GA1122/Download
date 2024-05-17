mount_stop (NautilusDirectory *directory)
{
    NautilusFile *file;

    if (directory->details->mount_state != NULL)
    {
        file = directory->details->mount_state->file;

        if (file != NULL)
        {
            g_assert (NAUTILUS_IS_FILE (file));
            g_assert (file->details->directory == directory);
            if (is_needy (file,
                          lacks_mount,
                          REQUEST_MOUNT))
            {
                return;
            }
        }

         
        mount_cancel (directory);
    }
}
