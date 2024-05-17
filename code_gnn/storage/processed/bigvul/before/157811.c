bool WebContentsImpl::NeedToFireBeforeUnload() {
  if (ShowingInterstitialPage())
    return false;

  if (!WillNotifyDisconnection())
    return false;

  if (GetRenderViewHost()->SuddenTerminationAllowed())
    return false;

  if (!GetMainFrame()->GetProcess()->SuddenTerminationAllowed())
    return true;

  return GetMainFrame()->ShouldDispatchBeforeUnload(
      true  );
}
