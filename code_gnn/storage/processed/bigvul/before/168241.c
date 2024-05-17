void BrowserView::CutCopyPaste(int command_id) {
#if defined(OS_MACOSX)
  ForwardCutCopyPasteToNSApp(command_id);
#else
  WebContents* contents = browser_->tab_strip_model()->GetActiveWebContents();
  if (contents) {
    void (WebContents::*method)();
    if (command_id == IDC_CUT)
      method = &content::WebContents::Cut;
    else if (command_id == IDC_COPY)
      method = &content::WebContents::Copy;
    else
      method = &content::WebContents::Paste;
    if (DoCutCopyPasteForWebContents(contents, method))
      return;

    WebContents* devtools =
        DevToolsWindow::GetInTabWebContents(contents, nullptr);
    if (devtools && DoCutCopyPasteForWebContents(devtools, method))
      return;
  }

  ui::Accelerator accelerator;
  GetAccelerator(command_id, &accelerator);
  GetFocusManager()->ProcessAccelerator(accelerator);
 #endif   
 }
