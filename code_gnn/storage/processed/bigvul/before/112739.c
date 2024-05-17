void PrintPreviewHandler::ClearInitiatorTabDetails() {
  TabContents* initiator_tab = GetInitiatorTab();
  if (!initiator_tab)
    return;

  printing::PrintPreviewTabController* tab_controller =
      printing::PrintPreviewTabController::GetInstance();
  if (tab_controller)
    tab_controller->EraseInitiatorTabInfo(preview_tab_contents());
}
