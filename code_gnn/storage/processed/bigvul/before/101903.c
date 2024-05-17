printing::PrintDialogGtkInterface* PrintDialogGtk::CreatePrintDialog(
    PrintingContextCairo* context) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  return new PrintDialogGtk(context);
}
