    DidNavigateMainFrame(const content::LoadCommittedDetails& details,
                         const content::FrameNavigateParams& params) {
  devtools_bindings_->DidNavigateMainFrame();
}
