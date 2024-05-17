  explicit AllowDialogIPCOnCommitFilter(WebContents* web_contents)
      : BrowserMessageFilter(FrameMsgStart),
        render_frame_host_(web_contents->GetMainFrame()) {
    web_contents_observer_.Observe(web_contents);
  }
