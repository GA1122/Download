void PrintPreviewHandler::HandlePrintWithCloudPrint(const ListValue*  ) {
  ReportStats();

  PrintPreviewUI* print_preview_ui = static_cast<PrintPreviewUI*>(
      web_ui()->GetController());
  scoped_refptr<base::RefCountedBytes> data;
  print_preview_ui->GetPrintPreviewDataForIndex(
      printing::COMPLETE_PREVIEW_DOCUMENT_INDEX, &data);
  if (!data.get()) {
    NOTREACHED();
    return;
  }
  DCHECK_GT(data->size(), 0U);

  gfx::NativeWindow modal_parent =
      platform_util::GetTopLevel(preview_web_contents()->GetNativeView());
  print_dialog_cloud::CreatePrintDialogForBytes(
      preview_web_contents()->GetBrowserContext(),
      modal_parent,
      data,
      string16(print_preview_ui->initiator_tab_title()),
      string16(),
      std::string("application/pdf"));

  ActivateInitiatorTabAndClosePreviewTab();
}
