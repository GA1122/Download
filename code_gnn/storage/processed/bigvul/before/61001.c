start_or_stop_io (NautilusDirectory *directory)
{
    NautilusFile *file;
    gboolean doing_io;

     
    file_list_start_or_stop (directory);

     
    file_info_stop (directory);
    directory_count_stop (directory);
    deep_count_stop (directory);
    mime_list_stop (directory);
    link_info_stop (directory);
    extension_info_stop (directory);
    mount_stop (directory);
    thumbnail_stop (directory);
    filesystem_info_stop (directory);

    doing_io = FALSE;
     
    while (!nautilus_file_queue_is_empty (directory->details->high_priority_queue))
    {
        file = nautilus_file_queue_head (directory->details->high_priority_queue);

         
        file_info_start (directory, file, &doing_io);
        link_info_start (directory, file, &doing_io);

        if (doing_io)
        {
            return;
        }

        move_file_to_low_priority_queue (directory, file);
    }

     
    while (!nautilus_file_queue_is_empty (directory->details->low_priority_queue))
    {
        file = nautilus_file_queue_head (directory->details->low_priority_queue);

         
        mount_start (directory, file, &doing_io);
        directory_count_start (directory, file, &doing_io);
        deep_count_start (directory, file, &doing_io);
        mime_list_start (directory, file, &doing_io);
        thumbnail_start (directory, file, &doing_io);
        filesystem_info_start (directory, file, &doing_io);

        if (doing_io)
        {
            return;
        }

        move_file_to_extension_queue (directory, file);
    }

     
    while (!nautilus_file_queue_is_empty (directory->details->extension_queue))
    {
        file = nautilus_file_queue_head (directory->details->extension_queue);

         
        extension_info_start (directory, file, &doing_io);
        if (doing_io)
        {
            return;
        }

        nautilus_directory_remove_file_from_work_queue (directory, file);
    }
}
