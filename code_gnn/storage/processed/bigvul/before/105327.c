void AutofillManager::DidNavigateMainFramePostCommit(
    const content::LoadCommittedDetails& details,
    const ViewHostMsg_FrameNavigate_Params& params) {
  Reset();
}
