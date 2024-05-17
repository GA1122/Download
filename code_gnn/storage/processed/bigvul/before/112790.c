void PrintPreviewUI::OnHidePreviewTab() {
  TabContents* preview_tab =
      TabContents::FromWebContents(web_ui()->GetWebContents());
  printing::BackgroundPrintingManager* background_printing_manager =
      g_browser_process->background_printing_manager();
  if (background_printing_manager->HasPrintPreviewTab(preview_tab))
    return;

  ConstrainedWebDialogDelegate* delegate = GetConstrainedDelegate();
  if (!delegate)
    return;
  delegate->ReleaseTabContentsOnDialogClose();
  background_printing_manager->OwnPrintPreviewTab(preview_tab);
  OnClosePrintPreviewTab();
}
