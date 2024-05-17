  void VerifyProcessIsBackgrounded(WebContents* web_contents) {
    constexpr bool kExpectedIsBackground = true;
    VerifyProcessPriority(web_contents->GetMainFrame()->GetProcess(),
                          kExpectedIsBackground);
  }
