TabMutedResult SetTabAudioMuted(content::WebContents* contents,
                                bool mute,
                                TabMutedReason reason,
                                const std::string& extension_id) {
  DCHECK(contents);
  DCHECK(TabMutedReason::NONE != reason);

  if (reason == TabMutedReason::AUDIO_INDICATOR &&
      !AreExperimentalMuteControlsEnabled()) {
    return TabMutedResult::FAIL_NOT_ENABLED;
  }

  if (!chrome::CanToggleAudioMute(contents))
    return TabMutedResult::FAIL_TABCAPTURE;

  contents->SetAudioMuted(mute);

  LastMuteMetadata::CreateForWebContents(contents);   
  LastMuteMetadata* const metadata =
      LastMuteMetadata::FromWebContents(contents);
  metadata->reason = reason;
  if (reason == TabMutedReason::EXTENSION) {
    DCHECK(!extension_id.empty());
    metadata->extension_id = extension_id;
  } else {
    metadata->extension_id.clear();
  }

  return TabMutedResult::SUCCESS;
}
