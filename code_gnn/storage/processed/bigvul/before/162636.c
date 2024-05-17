void HeadlessPrintManager::GetPDFContents(content::RenderFrameHost* rfh,
                                          const HeadlessPrintSettings& settings,
                                          const GetPDFCallback& callback) {
  DCHECK(callback);

  if (callback_) {
    callback.Run(SIMULTANEOUS_PRINT_ACTIVE, std::string());
    return;
  }
  printing_rfh_ = rfh;
  callback_ = callback;
  print_params_ = GetPrintParamsFromSettings(settings);
  page_ranges_text_ = settings.page_ranges;
  ignore_invalid_page_ranges_ = settings.ignore_invalid_page_ranges;
  rfh->Send(new PrintMsg_PrintPages(rfh->GetRoutingID()));
}
