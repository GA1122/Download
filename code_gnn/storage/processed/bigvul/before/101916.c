void PrintDialogGtk::UseDefaultSettings() {
  DCHECK(!page_setup_);

  gtk_settings_ = gtk_print_settings_new();

  scoped_ptr<GtkPrinterList> printer_list(new GtkPrinterList);
  printer_ = printer_list->default_printer();
  if (printer_) {
    g_object_ref(printer_);
    gtk_print_settings_set_printer(gtk_settings_,
                                   gtk_printer_get_name(printer_));
    page_setup_ = gtk_printer_get_default_page_size(printer_);
  }

  if (!page_setup_)
    page_setup_ = gtk_page_setup_new();

  PageRanges ranges_vector;
  InitPrintSettings(ranges_vector);
}
