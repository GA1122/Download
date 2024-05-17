void PrintPreviewMessageHandler::OnRequestPrintPreview(
    bool source_is_modifiable, bool webnode_only) {
  TabContentsWrapper* tab = tab_contents_wrapper();
  if (webnode_only)
    tab->print_view_manager()->PrintPreviewForWebNode();
  PrintPreviewTabController::PrintPreview(tab);
  PrintPreviewUI::SetSourceIsModifiable(GetPrintPreviewTab(),
                                        source_is_modifiable);
}
