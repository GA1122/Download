lacks_mount (NautilusFile *file)
{
    return (!file->details->mount_is_up_to_date &&
            (
                 
                file->details->is_mountpoint ||

                 
                (file->details->type == G_FILE_TYPE_DIRECTORY &&
                 nautilus_file_is_self_owned (file)) ||

                 
                (file->details->type == G_FILE_TYPE_MOUNTABLE)

            )
            );
}
