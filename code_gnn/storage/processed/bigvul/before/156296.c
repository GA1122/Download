media::AudioCodec CodecIdToMediaAudioCodec(AudioTrackRecorder::CodecId id) {
  switch (id) {
    case AudioTrackRecorder::CodecId::PCM:
      return media::kCodecPCM;
    case AudioTrackRecorder::CodecId::OPUS:
      return media::kCodecOpus;
    case AudioTrackRecorder::CodecId::LAST:
      return media::kUnknownAudioCodec;
  }
  NOTREACHED() << "Unsupported audio codec";
  return media::kUnknownAudioCodec;
}
