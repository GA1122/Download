  void SetAudioType(MediaStreamType audio_type) {
    DCHECK(IsAudioInputMediaType(audio_type) ||
           audio_type == MEDIA_NO_SERVICE);
    audio_type_ = audio_type;
  }
