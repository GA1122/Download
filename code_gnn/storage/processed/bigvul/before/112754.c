void PrintPreviewHandler::HandleManagePrinters(const ListValue*  ) {
  ++manage_printers_dialog_request_count_;
  printing::PrinterManagerDialog::ShowPrinterManagerDialog();
}
