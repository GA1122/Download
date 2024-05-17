void HTMLMediaElement::createPlaceholderTracksIfNecessary() {
  if (!mediaTracksEnabledInternally())
    return;

  if (hasAudio() && !audioTracks().length()) {
    addAudioTrack("audio", WebMediaPlayerClient::AudioTrackKindMain,
                  "Audio Track", "", false);
  }

  if (hasVideo() && !videoTracks().length()) {
    addVideoTrack("video", WebMediaPlayerClient::VideoTrackKindMain,
                  "Video Track", "", false);
  }
}
