  int PDFiumEngine::GetVisiblePageIndex(FPDF_PAGE page) {
  for (int page_index : visible_pages_) {
//    
//    
//   std::vector<int> visible_pages_copy(visible_pages_);
//   for (int page_index : visible_pages_copy) {
      if (pages_[page_index]->GetPage() == page)
        return page_index;
    }
   return -1;
 }