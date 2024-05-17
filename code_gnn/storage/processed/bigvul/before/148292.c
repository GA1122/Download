bool PrintViewManager::PrintForSystemDialogNow(
    const base::Closure& dialog_shown_callback) {
  DCHECK(!dialog_shown_callback.is_null());
  DCHECK(on_print_dialog_shown_callback_.is_null());
  on_print_dialog_shown_callback_ = dialog_shown_callback;

  SetPrintingRFH(print_preview_rfh_);
  int32_t id = print_preview_rfh_->GetRoutingID();
  return PrintNowInternal(print_preview_rfh_,
                          base::MakeUnique<PrintMsg_PrintForSystemDialog>(id));
}
