 PrintDialogGtk::PrintDialogGtk(const FilePath& path_to_pdf)
    : path_to_pdf_(path_to_pdf),
      browser_(BrowserList::GetLastActive()) {
  GtkWindow* parent = browser_->window()->GetNativeHandle();

  dialog_ = gtk_print_unix_dialog_new(NULL, parent);
  g_signal_connect(dialog_, "response", G_CALLBACK(OnResponseThunk), this);

  gtk_widget_show(dialog_);
 }
