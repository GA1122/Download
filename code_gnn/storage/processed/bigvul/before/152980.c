void PDFiumEngine::Form_ExecuteNamedAction(FPDF_FORMFILLINFO* param,
                                           FPDF_BYTESTRING named_action) {
  PDFiumEngine* engine = static_cast<PDFiumEngine*>(param);
  std::string action(named_action);
  if (action == "Print") {
    engine->client_->Print();
    return;
  }

  int index = engine->last_page_mouse_down_;
   
  if (index == -1)
    return;

  if (action == "NextPage") {
    engine->ScrollToPage(index + 1);
  } else if (action == "PrevPage") {
    engine->ScrollToPage(index - 1);
  } else if (action == "FirstPage") {
    engine->ScrollToPage(0);
  } else if (action == "LastPage") {
    engine->ScrollToPage(engine->pages_.size() - 1);
  }
}
