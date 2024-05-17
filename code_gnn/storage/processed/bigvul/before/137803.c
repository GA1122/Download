TabMutedReason GetTabAudioMutedReason(content::WebContents* contents) {
  LastMuteMetadata::CreateForWebContents(contents);   
  LastMuteMetadata* const metadata =
      LastMuteMetadata::FromWebContents(contents);
  if (GetTabAlertStateForContents(contents) == TabAlertState::TAB_CAPTURING) {
    metadata->reason = TabMutedReason::MEDIA_CAPTURE;
    metadata->extension_id.clear();
  }
  return metadata->reason;
}
