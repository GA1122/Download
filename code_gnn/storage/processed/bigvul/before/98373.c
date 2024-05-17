static void draw_page_callback(GtkPrintOperation* op, GtkPrintContext* context, gint page_nr, gpointer user_data)
{
    PrintContext* printContext = reinterpret_cast<PrintContext*>(user_data);

    if (page_nr >= printContext->pageCount())
        return;

    cairo_t* cr = gtk_print_context_get_cairo_context(context);
    GraphicsContext ctx(cr);
    float width = gtk_print_context_get_width(context);
    printContext->spoolPage(ctx, page_nr, width);
}
