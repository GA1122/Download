move_file_to_extension_queue (NautilusDirectory *directory,
                              NautilusFile      *file)
{
     
    nautilus_file_queue_enqueue (directory->details->extension_queue,
                                 file);
    nautilus_file_queue_remove (directory->details->low_priority_queue,
                                file);
}
