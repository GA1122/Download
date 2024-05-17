void PrintDialogGtk::SendDocumentToPrinter(const string16& document_name) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (!printer_) {
    Release();
    return;
  }

  GtkPrintJob* print_job = gtk_print_job_new(
      UTF16ToUTF8(document_name).c_str(),
      printer_,
      gtk_settings_,
      page_setup_);
  gtk_print_job_set_source_file(print_job, path_to_pdf_.value().c_str(), NULL);
  gtk_print_job_send(print_job, OnJobCompletedThunk, this, NULL);
}
