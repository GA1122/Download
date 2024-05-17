void PrepContentsForBeforeUnloadTest(WebContents* web_contents) {
  for (auto* frame : web_contents->GetAllFrames()) {
    frame->ExecuteJavaScriptWithUserGestureForTests(base::string16());

    frame->DisableBeforeUnloadHangMonitorForTesting();
  }
}
