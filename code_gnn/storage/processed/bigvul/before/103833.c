void RenderView::OnSetInLiveResize(bool in_live_resize) {
    if (!webview())
        return;
    if (in_live_resize)
        webview()->willStartLiveResize();
    else
        webview()->willEndLiveResize();
}
