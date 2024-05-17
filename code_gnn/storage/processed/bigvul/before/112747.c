void PrintPreviewHandler::HandleCancelPendingPrintRequest(
    const ListValue*  ) {
  TabContents* initiator_tab = GetInitiatorTab();
  if (initiator_tab)
    ClearInitiatorTabDetails();
  gfx::NativeWindow parent = initiator_tab ?
      initiator_tab->web_contents()->GetView()->GetTopLevelNativeWindow() :
      NULL;
  chrome::ShowPrintErrorDialog(parent);
}
