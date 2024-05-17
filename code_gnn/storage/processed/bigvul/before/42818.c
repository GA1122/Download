static void gtk_entry_emit_dialog_response_ok(GtkEntry *entry, GtkDialog *dialog)
{
     
    if (gtk_entry_get_text_length(entry) > 0)
        gtk_dialog_response(dialog, GTK_RESPONSE_OK);
}
