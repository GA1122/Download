void FrameLoader::UpdateForSameDocumentNavigation(
    const KURL& new_url,
    SameDocumentNavigationSource same_document_navigation_source,
    scoped_refptr<SerializedScriptValue> data,
    HistoryScrollRestorationType scroll_restoration_type,
    WebFrameLoadType type,
    Document* initiating_document) {
  SinglePageAppNavigationType single_page_app_navigation_type =
      CategorizeSinglePageAppNavigation(same_document_navigation_source, type);
  UMA_HISTOGRAM_ENUMERATION(
      "RendererScheduler.UpdateForSameDocumentNavigationCount",
      single_page_app_navigation_type, kSPANavTypeCount);

  TRACE_EVENT1("blink", "FrameLoader::updateForSameDocumentNavigation", "url",
               new_url.GetString().Ascii().data());

  bool was_loading = frame_->IsLoading();
  if (!was_loading)
    Client()->DidStartLoading();

  frame_->GetDocument()->SetURL(new_url);
  GetDocumentLoader()->UpdateForSameDocumentNavigation(
      new_url, same_document_navigation_source, std::move(data),
      scroll_restoration_type, type, initiating_document);
  if (!was_loading)
    Client()->DidStopLoading();
}
