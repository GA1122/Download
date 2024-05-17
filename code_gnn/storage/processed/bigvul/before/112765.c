void PrintPreviewHandler::PostPrintToPdfTask(base::RefCountedBytes* data) {
  if (!data) {
    NOTREACHED();
    return;
  }
  printing::PreviewMetafile* metafile = new printing::PreviewMetafile;
  metafile->InitFromData(static_cast<const void*>(data->front()), data->size());
#ifdef OS_CHROMEOS
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  gdata::util::PrepareWritableFileAndRun(
      Profile::FromBrowserContext(preview_web_contents()->GetBrowserContext()),
      *print_to_pdf_path_,
      base::Bind(&PrintToPdfCallbackWithCheck, metafile));
#else
  BrowserThread::PostTask(BrowserThread::FILE, FROM_HERE,
                          base::Bind(&PrintToPdfCallback, metafile,
                                     *print_to_pdf_path_));
#endif

  print_to_pdf_path_.reset();
  ActivateInitiatorTabAndClosePreviewTab();
}
