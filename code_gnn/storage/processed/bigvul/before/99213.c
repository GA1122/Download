void PrintDialogGtk::OnJobCompleted(GtkPrintJob* job, GError* error) {
  gtk_widget_destroy(dialog_);

  if (error)
    LOG(ERROR) << "Printing failed: " << error->message;

  if (job)
    g_object_unref(job);

  file_util::Delete(path_to_pdf_, false);

  delete this;
}
