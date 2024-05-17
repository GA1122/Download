void PrintPreviewMessageHandler::NavigateToPendingEntry(
    const GURL& url,
    NavigationController::ReloadType reload_type) {
  TabContentsWrapper* tab = tab_contents_wrapper();
  TabContentsWrapper* preview_tab = GetPrintPreviewTab();
  if (tab == preview_tab) {
    DCHECK(PrintPreviewTabController::IsPrintPreviewURL(url));
    DCHECK_EQ(NavigationController::RELOAD, reload_type);
    return;
  }
  if (preview_tab)
    tab->print_view_manager()->PrintPreviewDone();
}
