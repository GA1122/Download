void Browser::RendererUnresponsive(WebContents* source) {
  TabContents* tab_contents = TabContents::FromWebContents(source);
  if (tab_contents) {
    printing::PrintPreviewTabController* controller =
        printing::PrintPreviewTabController::GetInstance();
    if (controller) {
      TabContents* preview_tab =
          controller->GetPrintPreviewForTab(tab_contents);
      if (preview_tab && preview_tab != tab_contents) {
        return;
      }
    }
  }

  chrome::ShowHungRendererDialog(source);
}
