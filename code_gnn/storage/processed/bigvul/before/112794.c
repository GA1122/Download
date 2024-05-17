 void PrintPreviewUI::OnPrintPreviewTabClosed() {
  TabContents* preview_tab =
      TabContents::FromWebContents(web_ui()->GetWebContents());
  printing::BackgroundPrintingManager* background_printing_manager =
      g_browser_process->background_printing_manager();
  if (background_printing_manager->HasPrintPreviewTab(preview_tab))
    return;
  OnClosePrintPreviewTab();
}