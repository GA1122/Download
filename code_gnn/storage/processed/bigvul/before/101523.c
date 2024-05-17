void PrintWebViewHelper::Print(WebKit::WebFrame* frame,
                               const WebKit::WebNode& node) {
  if (print_web_view_)
    return;

  scoped_ptr<PrepareFrameAndViewForPrint> prepare;
  if (!InitPrintSettingsAndPrepareFrame(frame, node, &prepare)) {
    DidFinishPrinting(FAIL_PRINT);
    return;   
  }

  int expected_page_count = 0;
  bool use_browser_overlays = true;

  expected_page_count = prepare->GetExpectedPageCount();
  if (expected_page_count)
    use_browser_overlays = prepare->ShouldUseBrowserOverlays();

  prepare.reset();

  if (!expected_page_count) {
    DidFinishPrinting(OK);   
    return;
  }

  if (!GetPrintSettingsFromUser(frame, node, expected_page_count,
                                use_browser_overlays)) {
    DidFinishPrinting(OK);   
    return;
  }

  if (!RenderPagesForPrint(frame, node, NULL)) {
    LOG(ERROR) << "RenderPagesForPrint failed";
    DidFinishPrinting(FAIL_PRINT);
  }
  ResetScriptedPrintCount();
}
