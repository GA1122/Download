void PrintWebViewHelper::Print(blink::WebLocalFrame* frame,
                               const blink::WebNode& node,
                               bool is_scripted) {
  if (prep_frame_view_)
    return;

  FrameReference frame_ref(frame);

  int expected_page_count = 0;
  if (!CalculateNumberOfPages(frame, node, &expected_page_count)) {
    DidFinishPrinting(FAIL_PRINT_INIT);
    return;   
  }

  if (!expected_page_count) {
    DidFinishPrinting(FAIL_PRINT);
    return;
  }

  if (delegate_->IsAskPrintSettingsEnabled() &&
      !GetPrintSettingsFromUser(frame_ref.GetFrame(), node, expected_page_count,
                                is_scripted)) {
    DidFinishPrinting(OK);   
    return;
  }

  if (!RenderPagesForPrint(frame_ref.GetFrame(), node)) {
    LOG(ERROR) << "RenderPagesForPrint failed";
    DidFinishPrinting(FAIL_PRINT);
  }
  scripting_throttler_.Reset();
}
