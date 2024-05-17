  GtkPrinterList() : default_printer_(NULL) {
    gtk_enumerate_printers((GtkPrinterFunc)SetPrinter, this, NULL, TRUE);
  }
