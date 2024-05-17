void PrintDialogGtk::CreatePrintDialogForPdf(const FilePath& path) {
  ChromeThread::PostTask(
      ChromeThread::UI, FROM_HERE,
       NewRunnableFunction(&PrintDialogGtk::CreateDialogImpl, path));
 }
