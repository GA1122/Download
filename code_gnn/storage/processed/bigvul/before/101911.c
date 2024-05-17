void PrintDialogGtk::PrintDocument(const printing::Metafile* metafile,
                                   const string16& document_name) {
  DCHECK(!BrowserThread::CurrentlyOn(BrowserThread::UI));

  AddRef();

  bool error = false;
  if (!file_util::CreateTemporaryFile(&path_to_pdf_)) {
    LOG(ERROR) << "Creating temporary file failed";
    error = true;
  }

  if (!error && !metafile->SaveTo(path_to_pdf_)) {
    LOG(ERROR) << "Saving metafile failed";
    file_util::Delete(path_to_pdf_, false);
    error = true;
  }

  if (error) {
    Release();
  } else {
    BrowserThread::PostTask(
        BrowserThread::UI, FROM_HERE,
        NewRunnableMethod(this,
                          &PrintDialogGtk::SendDocumentToPrinter,
                          document_name));
  }
}
