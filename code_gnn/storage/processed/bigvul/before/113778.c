void PrintPreviewUI::OnPrintPreviewTabClosed() {
  TabContentsWrapper* preview_tab =
      TabContentsWrapper::GetCurrentWrapperForContents(
        web_ui()->GetWebContents());
  printing::BackgroundPrintingManager* background_printing_manager =
      g_browser_process->background_printing_manager();
  if (background_printing_manager->HasPrintPreviewTab(preview_tab))
    return;
  OnClosePrintPreviewTab();
}
