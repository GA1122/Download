bool WebLocalFrameImpl::HasCustomPageSizeStyle(int page_index) {
  return GetFrame()->GetDocument()->StyleForPage(page_index)->PageSizeType() !=
         EPageSizeType::kAuto;
}
