void GuestViewBase::DidNavigateMainFrame(
    const content::LoadCommittedDetails& details,
    const content::FrameNavigateParams& params) {
  if (attached() && ZoomPropagatesFromEmbedderToGuest())
    SetGuestZoomLevelToMatchEmbedder();
}
