void UnmuteIfMutedByExtension(content::WebContents* contents,
                              const std::string& extension_id) {
  LastMuteMetadata::CreateForWebContents(contents);   
  LastMuteMetadata* const metadata =
      LastMuteMetadata::FromWebContents(contents);
  if (metadata->reason == TabMutedReason::EXTENSION &&
      metadata->extension_id == extension_id) {
    SetTabAudioMuted(contents, false, TabMutedReason::EXTENSION, extension_id);
  }
}
