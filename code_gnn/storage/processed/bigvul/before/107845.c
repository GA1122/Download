void PrintWebViewHelper::Print(WebKit::WebFrame* frame,
                               WebNode* node,
                               bool script_initiated,
                               bool is_preview) {
  const int kMinSecondsToIgnoreJavascriptInitiatedPrint = 2;
  const int kMaxSecondsToIgnoreJavascriptInitiatedPrint = 2 * 60;   

  if (IsPrinting())
    return;

  if (script_initiated && (user_cancelled_scripted_print_count_ > 0)) {
    base::TimeDelta diff = base::Time::Now() - last_cancelled_script_print_;
    int min_wait_seconds = std::min(
        kMinSecondsToIgnoreJavascriptInitiatedPrint <<
            (user_cancelled_scripted_print_count_ - 1),
        kMaxSecondsToIgnoreJavascriptInitiatedPrint);
    if (diff.InSeconds() < min_wait_seconds) {
      WebString message(WebString::fromUTF8(
          "Ignoring too frequent calls to print()."));
      frame->addMessageToConsole(WebConsoleMessage(
          WebConsoleMessage::LevelWarning,
          message));
      return;
    }
  }

  bool print_cancelled = false;
  is_preview_ = is_preview;

  if (!InitPrintSettings(frame, node))
    return;   

  int expected_pages_count = 0;
  bool use_browser_overlays = true;

  {
    PrepareFrameAndViewForPrint prep_frame_view(
        (*print_pages_params_).params, frame, node, frame->view());
    expected_pages_count = prep_frame_view.GetExpectedPageCount();
    if (expected_pages_count)
      use_browser_overlays = prep_frame_view.ShouldUseBrowserOverlays();
  }

  if (expected_pages_count) {
    if (!is_preview_) {
      if (!GetPrintSettingsFromUser(frame, expected_pages_count,
                                    use_browser_overlays)) {
        print_cancelled = true;
      }
    }

    if (!print_cancelled) {
      if (is_preview_)
        RenderPagesForPreview(frame);
      else
        RenderPagesForPrint(frame, node);

      user_cancelled_scripted_print_count_ = 0;
      return;   
    } else {
      if (script_initiated) {
        ++user_cancelled_scripted_print_count_;
        last_cancelled_script_print_ = base::Time::Now();
      }
    }
  } else {
    print_cancelled = true;
  }
  if (print_cancelled)
    DidFinishPrinting(print_cancelled);
}
