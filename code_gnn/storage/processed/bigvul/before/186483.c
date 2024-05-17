 int PDFiumEngine::GetMostVisiblePage() {
    if (in_flight_visible_page_)
      return *in_flight_visible_page_;
  
//    
//    
//    
//    
//   defer_page_unload_ = true;
    CalculateVisiblePages();
//   defer_page_unload_ = false;
    return most_visible_page_;
  }