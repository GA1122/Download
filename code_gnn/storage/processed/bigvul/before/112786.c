void PrintPreviewUI::OnClosePrintPreviewTab() {
  if (tab_closed_)
    return;
  tab_closed_ = true;
  ConstrainedWebDialogDelegate* delegate = GetConstrainedDelegate();
  if (!delegate)
    return;
  delegate->GetWebDialogDelegate()->OnDialogClosed("");
  delegate->OnDialogCloseFromWebUI();
}
