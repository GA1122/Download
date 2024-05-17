void WebContentsImpl::OnOpenDateTimeDialog(
    const ViewHostMsg_DateTimeDialogValue_Params& value) {
  date_time_chooser_->ShowDialog(GetTopLevelNativeWindow(),
                                 GetRenderViewHost(),
                                 value.dialog_type,
                                 value.dialog_value,
                                 value.minimum,
                                 value.maximum,
                                 value.step,
                                 value.suggestions);
}
