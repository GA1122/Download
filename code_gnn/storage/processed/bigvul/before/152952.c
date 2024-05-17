void PDFiumEngine::AppendPage(PDFEngine* engine, int index) {
  pages_[index]->Unload();
  pages_[index]->set_calculated_links(false);
  pp::Size curr_page_size = GetPageSize(index);
  FPDFPage_Delete(doc_, index);
  FPDF_ImportPages(doc_,
                   static_cast<PDFiumEngine*>(engine)->doc(),
                   "1",
                   index);
  pp::Size new_page_size = GetPageSize(index);
  if (curr_page_size != new_page_size)
    LoadPageInfo(true);
  client_->Invalidate(GetPageScreenRect(index));
}
