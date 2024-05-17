void PrintWebViewHelper::PrintPage(blink::WebLocalFrame* frame,
                                   bool user_initiated) {
  DCHECK(frame);

  if (delegate_->CancelPrerender(render_view(), routing_id()))
    return;

  if (!IsScriptInitiatedPrintAllowed(frame, user_initiated))
    return;

  if (delegate_->OverridePrint(frame))
    return;

  if (!g_is_preview_enabled_) {
    Print(frame, blink::WebNode(), true);
  } else {
    print_preview_context_.InitWithFrame(frame);
    RequestPrintPreview(PRINT_PREVIEW_SCRIPTED);
  }
 }
