void PrintWebViewHelper::PrintFrame(WebFrame* frame,
                                    bool script_initiated,
                                    bool is_preview) {
  Print(frame, NULL, script_initiated, is_preview);
}
