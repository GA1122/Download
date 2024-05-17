get_application_no_mime_type_handler_message (NautilusFile *file,
                                              char         *uri)
{
    char *uri_for_display;
    char *name;
    char *error_message;

    name = nautilus_file_get_display_name (file);

     
    uri_for_display = eel_str_middle_truncate (name, MAX_URI_IN_DIALOG_LENGTH);
    error_message = g_strdup_printf (_("Could not display “%s”."), uri_for_display);
    g_free (uri_for_display);
    g_free (name);

    return error_message;
}
