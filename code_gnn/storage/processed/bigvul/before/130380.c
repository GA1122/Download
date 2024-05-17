void SearchEngineTabHelper::DidNavigateMainFrame(
    const content::LoadCommittedDetails&  ,
    const content::FrameNavigateParams& params) {
  GenerateKeywordIfNecessary(params);
}
