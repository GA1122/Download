void PrintWebViewHelper::PrintPage(WebKit::WebFrame* frame) {
  DCHECK(frame);

  if (prerender::PrerenderHelper::IsPrerendering(render_view())) {
    Send(new ChromeViewHostMsg_CancelPrerenderForPrinting(routing_id()));
    return;
  }

  if (!IsScriptInitiatedPrintAllowed(frame))
    return;
  IncrementScriptedPrintCount();

  if (is_preview_enabled_) {
    print_preview_context_.InitWithFrame(frame);
    RequestPrintPreview(PRINT_PREVIEW_SCRIPTED);
  } else {
    Print(frame, WebNode());
  }
}
