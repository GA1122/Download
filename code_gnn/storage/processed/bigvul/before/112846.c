  void OnPrintPreview(const DictionaryValue& dict) {
    PrintWebViewHelper* print_web_view_helper = PrintWebViewHelper::Get(view_);
    print_web_view_helper->OnInitiatePrintPreview();
    print_web_view_helper->OnPrintPreview(dict);
  }
