void PrintPreviewHandler::OnTabDestroyed() {
  TabContents* initiator_tab = GetInitiatorTab();
  if (!initiator_tab)
    return;

  initiator_tab->print_view_manager()->set_observer(NULL);
}
