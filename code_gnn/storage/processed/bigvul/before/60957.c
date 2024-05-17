move_file_to_low_priority_queue (NautilusDirectory *directory,
                                 NautilusFile      *file)
{
     
    nautilus_file_queue_enqueue (directory->details->low_priority_queue,
                                 file);
    nautilus_file_queue_remove (directory->details->high_priority_queue,
                                file);
}
