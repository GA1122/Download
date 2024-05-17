TabContentsWrapper* PrintPreviewMessageHandler::GetPrintPreviewTab() {
  PrintPreviewTabController* tab_controller =
      PrintPreviewTabController::GetInstance();
  if (!tab_controller)
    return NULL;

  return tab_controller->GetPrintPreviewForTab(tab_contents_wrapper());
}
