  GtkPrinter* GetPrinterWithName(const char* name) {
    if (!name || !*name)
      return NULL;

    for (std::vector<GtkPrinter*>::iterator it = printers_.begin();
         it < printers_.end(); ++it) {
      if (strcmp(name, gtk_printer_get_name(*it)) == 0) {
        return *it;
      }
    }

    return NULL;
  }
