void PrintDialogGtk::OnResponse(GtkWidget* dialog, int response_id) {
  gtk_widget_hide(dialog_);

  switch (response_id) {
    case GTK_RESPONSE_OK: {
      if (gtk_settings_)
        g_object_unref(gtk_settings_);
      gtk_settings_ = gtk_print_unix_dialog_get_settings(
          GTK_PRINT_UNIX_DIALOG(dialog_));

      if (printer_)
        g_object_unref(printer_);
      printer_ = gtk_print_unix_dialog_get_selected_printer(
          GTK_PRINT_UNIX_DIALOG(dialog_));
      g_object_ref(printer_);

      if (page_setup_)
        g_object_unref(page_setup_);
      page_setup_ = gtk_print_unix_dialog_get_page_setup(
          GTK_PRINT_UNIX_DIALOG(dialog_));
      g_object_ref(page_setup_);

      PageRanges ranges_vector;
      gint num_ranges;
      GtkPageRange* gtk_range =
          gtk_print_settings_get_page_ranges(gtk_settings_, &num_ranges);
      if (gtk_range) {
        for (int i = 0; i < num_ranges; ++i) {
          printing::PageRange range;
          range.from = gtk_range[i].start;
          range.to = gtk_range[i].end;
          ranges_vector.push_back(range);
        }
        g_free(gtk_range);
      }

      PrintSettings settings;
      printing::PrintSettingsInitializerGtk::InitPrintSettings(
          gtk_settings_, page_setup_, ranges_vector, false, &settings);
      context_->InitWithSettings(settings);
      callback_->Run(PrintingContextCairo::OK);
      callback_ = NULL;
      return;
    }
    case GTK_RESPONSE_DELETE_EVENT:   
    case GTK_RESPONSE_CANCEL: {
      callback_->Run(PrintingContextCairo::CANCEL);
      callback_ = NULL;
      return;
    }
    case GTK_RESPONSE_APPLY:
    default: {
      NOTREACHED();
    }
  }
}
