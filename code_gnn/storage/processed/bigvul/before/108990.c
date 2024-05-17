void RenderViewImpl::OnSwapOut(const ViewMsg_SwapOut_Params& params) {
  OnStop();

  if (!is_swapped_out_) {

    SyncNavigationState();

    webview()->dispatchUnloadEvent();

    SetSwappedOut(true);

    NavigateToSwappedOutURL(webview()->mainFrame());

    webview()->setVisibilityState(WebKit::WebPageVisibilityStateHidden, false);
  }

  Send(new ViewHostMsg_SwapOut_ACK(routing_id_, params));
}
