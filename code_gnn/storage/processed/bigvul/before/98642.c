void RenderWidget::OnImeSetComposition(WebCompositionCommand command,
                                       int cursor_position,
                                       int target_start, int target_end,
                                       const string16& ime_string) {
  if (!webwidget_)
    return;
  ime_control_busy_ = true;
  if (!webwidget_->handleCompositionEvent(command, cursor_position,
      target_start, target_end, ime_string) &&
      command == WebKit::WebCompositionCommandSet) {
    Send(new ViewHostMsg_ImeUpdateStatus(routing_id(), IME_CANCEL_COMPOSITION,
                                         WebRect()));
  }
  ime_control_busy_ = false;
}
