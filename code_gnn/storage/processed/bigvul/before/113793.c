bool PrintWebViewHelper::InitPrintSettings() {
  PrintMsg_PrintPages_Params settings;
  Send(new PrintHostMsg_GetDefaultPrintSettings(routing_id(),
                                                &settings.params));
  bool result = true;
  if (!PrintMsg_Print_Params_IsValid(settings.params))
    result = false;

  if (result &&
      (settings.params.dpi < kMinDpi || settings.params.document_cookie == 0)) {
    NOTREACHED();
    result = false;
  }

  ignore_css_margins_ = false;
  settings.params.fit_to_paper_size = true;
  settings.pages.clear();

  print_pages_params_.reset(new PrintMsg_PrintPages_Params(settings));
  return result;
}
