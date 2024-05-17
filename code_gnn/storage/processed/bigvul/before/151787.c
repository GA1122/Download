void BackgroundLoaderOffliner::DocumentAvailableInMainFrame() {
  is_low_bar_met_ = true;

  AddLoadingSignal("DocumentAvailableInMainFrame");
}
