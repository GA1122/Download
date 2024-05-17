void PrintingContextCairo::PrintDocument(const Metafile* metafile) {
  DCHECK(print_dialog_);
  DCHECK(metafile);
  print_dialog_->PrintDocument(metafile, document_name_);
}
