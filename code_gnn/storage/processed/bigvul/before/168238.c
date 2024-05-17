void BrowserView::CloseAllTabsStopped(TabStripModel* tab_strip_model,
                                      CloseAllStoppedReason reason) {
  if (reason == kCloseAllCanceled)
    web_contents_close_handler_->CloseAllTabsCanceled();
}
