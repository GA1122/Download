activation_mountable_started (NautilusFile *file,
                              GFile        *gfile_of_file,
                              GError       *error,
                              gpointer      callback_data)
{
    ActivateParameters *parameters = callback_data;
    LaunchLocation *location;

     
    parameters->start_mountables = g_list_remove (parameters->start_mountables, file);
    nautilus_file_unref (file);

    if (error == NULL)
    {
         
        location = find_launch_location_for_file (parameters->locations, file);
        if (location != NULL)
        {
            parameters->locations = g_list_remove (parameters->locations, location);
            launch_location_free (location);
        }
    }
    else
    {
         
        if (error->domain != G_IO_ERROR ||
            (error->code != G_IO_ERROR_FAILED_HANDLED))
        {
            location = find_launch_location_for_file (parameters->locations,
                                                      file);
            if (location)
            {
                parameters->locations =
                    g_list_remove (parameters->locations,
                                   location);
                launch_location_free (location);
            }
        }

        if (error->domain != G_IO_ERROR ||
            (error->code != G_IO_ERROR_CANCELLED &&
             error->code != G_IO_ERROR_FAILED_HANDLED))
        {
            eel_show_error_dialog (_("Unable to start location"),
                                   error->message, NULL);
        }

        if (error->code == G_IO_ERROR_CANCELLED)
        {
            activation_parameters_free (parameters);
            return;
        }
    }

     
    activation_start_mountables (parameters);
}
