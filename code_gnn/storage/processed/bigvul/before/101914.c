  static bool SetPrinter(GtkPrinter* printer, GtkPrinterList* printer_list) {
    if (gtk_printer_is_default(printer))
      printer_list->default_printer_ = printer;

    g_object_ref(printer);
    printer_list->printers_.push_back(printer);

    return false;
  }
