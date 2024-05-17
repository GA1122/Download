void HTMLMediaElement::CreatePlaceholderTracksIfNecessary() {
  if (!MediaTracksEnabledInternally())
    return;

  if (HasAudio() && !audioTracks().length()) {
    AddAudioTrack("audio", WebMediaPlayerClient::kAudioTrackKindMain,
                  "Audio Track", "", true);
  }

  if (HasVideo() && !videoTracks().length()) {
    AddVideoTrack("video", WebMediaPlayerClient::kVideoTrackKindMain,
                  "Video Track", "", true);
  }
}
