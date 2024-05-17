bool WebContentsImpl::NeedToFireBeforeUnload() {
  return WillNotifyDisconnection() && !ShowingInterstitialPage() &&
         !GetRenderViewHost()->SuddenTerminationAllowed();
}
