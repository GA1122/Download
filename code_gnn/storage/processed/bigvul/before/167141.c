void HTMLMediaElement::CreatePlaceholderTracksIfNecessary() {
  if (!MediaTracksEnabledInternally())
    return;

  if (HasAudio() && !audioTracks().length()) {
    AddAudioTrack("audio", WebMediaPlayerClient::kAudioTrackKindMain,
                  "Audio Track", "", false);
  }

  if (HasVideo() && !videoTracks().length()) {
    AddVideoTrack("video", WebMediaPlayerClient::kVideoTrackKindMain,
                  "Video Track", "", false);
  }
}
