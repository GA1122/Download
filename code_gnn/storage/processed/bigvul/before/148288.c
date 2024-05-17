void PrintViewManager::OnDidShowPrintDialog(content::RenderFrameHost* rfh) {
  if (rfh != print_preview_rfh_)
    return;

  if (on_print_dialog_shown_callback_.is_null())
    return;

  on_print_dialog_shown_callback_.Run();
  on_print_dialog_shown_callback_.Reset();
}
