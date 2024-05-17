void RenderViewImpl::TransferActiveWheelFlingAnimation(
    const WebKit::WebActiveWheelFlingParameters& params) {
  if (webview())
    webview()->transferActiveWheelFlingAnimation(params);
}
