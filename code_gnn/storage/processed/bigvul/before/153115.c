void PDFiumEngine::SetCurrentPage(int index) {
  in_flight_visible_page_.reset();

  if (index == most_visible_page_ || !form_)
    return;

  if (most_visible_page_ != -1 && called_do_document_action_) {
    FPDF_PAGE old_page = pages_[most_visible_page_]->GetPage();
    FORM_DoPageAAction(old_page, form_, FPDFPAGE_AACTION_CLOSE);
  }
  most_visible_page_ = index;
#if defined(OS_LINUX)
    g_last_instance_id = client_->GetPluginInstance()->pp_instance();
#endif
  if (most_visible_page_ != -1 && called_do_document_action_) {
    FPDF_PAGE new_page = pages_[most_visible_page_]->GetPage();
    FORM_DoPageAAction(new_page, form_, FPDFPAGE_AACTION_OPEN);
  }
}
