media::VideoCodec CodecIdToMediaVideoCodec(VideoTrackRecorder::CodecId id) {
  switch (id) {
    case VideoTrackRecorder::CodecId::VP8:
      return media::kCodecVP8;
    case VideoTrackRecorder::CodecId::VP9:
      return media::kCodecVP9;
#if BUILDFLAG(RTC_USE_H264)
    case VideoTrackRecorder::CodecId::H264:
      return media::kCodecH264;
#endif
    case VideoTrackRecorder::CodecId::LAST:
      return media::kUnknownVideoCodec;
  }
  NOTREACHED() << "Unsupported video codec";
  return media::kUnknownVideoCodec;
}
