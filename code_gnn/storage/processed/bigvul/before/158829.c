void Browser::LoadingStateChanged(WebContents* source,
                                  bool to_different_document) {
  ScheduleUIUpdate(source, content::INVALIDATE_TYPE_LOAD);
  UpdateWindowForLoadingStateChanged(source, to_different_document);
}
