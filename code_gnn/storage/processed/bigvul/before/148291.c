void PrintViewManager::OnSetupScriptedPrintPreview(
    content::RenderFrameHost* rfh,
    IPC::Message* reply_msg) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  auto& map = g_scripted_print_preview_closure_map.Get();
  content::RenderProcessHost* rph = rfh->GetProcess();

  if (base::ContainsKey(map, rph)) {
    rfh->Send(reply_msg);
    return;
  }

  if (print_preview_state_ != NOT_PREVIEWING) {
    rfh->Send(reply_msg);
    return;
  }

  PrintPreviewDialogController* dialog_controller =
      PrintPreviewDialogController::GetInstance();
  if (!dialog_controller) {
    rfh->Send(reply_msg);
    return;
  }

  DCHECK(!print_preview_rfh_);
  print_preview_rfh_ = rfh;
  print_preview_state_ = SCRIPTED_PREVIEW;
  map[rph] = base::Bind(&PrintViewManager::OnScriptedPrintPreviewReply,
                        base::Unretained(this), reply_msg);
  scripted_print_preview_rph_ = rph;
}
