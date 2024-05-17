bool PrintWebViewHelper::InitPrintSettings(WebFrame* frame,
                                           WebNode* node) {
  ViewMsg_PrintPages_Params settings;
  if (GetDefaultPrintSettings(frame, node, &settings.params)) {
    print_pages_params_.reset(new ViewMsg_PrintPages_Params(settings));
    print_pages_params_->pages.clear();
    return true;
  }
  return false;
}
