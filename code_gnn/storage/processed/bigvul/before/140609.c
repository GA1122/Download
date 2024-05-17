void OutOfProcessInstance::DocumentSizeUpdated(const pp::Size& size) {
  document_size_ = size;

  pp::VarDictionary dimensions;
  dimensions.Set(kType, kJSDocumentDimensionsType);
  dimensions.Set(kJSDocumentWidth, pp::Var(document_size_.width()));
  dimensions.Set(kJSDocumentHeight, pp::Var(document_size_.height()));
  pp::VarArray page_dimensions_array;
  int num_pages = engine_->GetNumberOfPages();
  for (int i = 0; i < num_pages; ++i) {
    pp::Rect page_rect = engine_->GetPageRect(i);
    pp::VarDictionary page_dimensions;
    page_dimensions.Set(kJSPageX, pp::Var(page_rect.x()));
    page_dimensions.Set(kJSPageY, pp::Var(page_rect.y()));
    page_dimensions.Set(kJSPageWidth, pp::Var(page_rect.width()));
    page_dimensions.Set(kJSPageHeight, pp::Var(page_rect.height()));
    page_dimensions_array.Set(i, page_dimensions);
  }
  dimensions.Set(kJSPageDimensions, page_dimensions_array);
  PostMessage(dimensions);

  OnGeometryChanged(zoom_, device_scale_);
}
