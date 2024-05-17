void PrintPreviewHandler::FileSelected(const FilePath& path,
                                       int index, void* params) {
  GetStickySettings()->StoreSavePath(path.DirName());

  PrintPreviewUI* print_preview_ui = static_cast<PrintPreviewUI*>(
      web_ui()->GetController());
  print_preview_ui->web_ui()->CallJavascriptFunction("fileSelectionCompleted");
  scoped_refptr<base::RefCountedBytes> data;
  print_preview_ui->GetPrintPreviewDataForIndex(
      printing::COMPLETE_PREVIEW_DOCUMENT_INDEX, &data);
  print_to_pdf_path_.reset(new FilePath(path));
  PostPrintToPdfTask(data);
}
