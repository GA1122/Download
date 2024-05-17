void RenderViewImpl::PageScaleFactorChanged() {
  if (!webview())
    return;

  Send(new ViewHostMsg_PageScaleFactorChanged(GetRoutingID(),
                                              webview()->PageScaleFactor()));
}
