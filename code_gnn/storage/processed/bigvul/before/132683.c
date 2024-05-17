void BlinkTestRunner::SetScreenOrientation(
    const WebScreenOrientationType& orientation) {
  MockScreenOrientationClient* mock_client =
      proxy()->GetScreenOrientationClientMock();
  mock_client->UpdateDeviceOrientation(
      render_view()->GetWebView()->mainFrame()->toWebLocalFrame(), orientation);
}
