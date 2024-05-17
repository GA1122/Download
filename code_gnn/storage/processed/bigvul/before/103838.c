void RenderView::OnStop() {
  if (webview()) {
    WebFrame* main_frame = webview()->mainFrame();
    StopAltErrorPageFetcher(main_frame->provisionalDataSource());
    StopAltErrorPageFetcher(main_frame->dataSource());
    main_frame->stopLoading();
  }
}
