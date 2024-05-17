static void end_print_callback(GtkPrintOperation* op, GtkPrintContext* context, gpointer user_data)
{
    PrintContext* printContext = reinterpret_cast<PrintContext*>(user_data);
    printContext->end();
}
