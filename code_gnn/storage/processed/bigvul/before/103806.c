void RenderView::OnCopyToFindPboard() {
  if (!webview())
    return;

  WebFrame* frame = webview()->focusedFrame();
  if (frame->hasSelection()) {
    string16 selection = frame->selectionAsText();
    RenderThread::current()->Send(
        new ClipboardHostMsg_FindPboardWriteStringAsync(selection));
  }
}
