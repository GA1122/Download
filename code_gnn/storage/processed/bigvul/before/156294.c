AudioTrackRecorder::CodecId AudioStringToCodecId(
    const blink::WebString& codecs) {
  const std::string& codecs_str = ToLowerASCII(codecs.Utf8());

  if (codecs_str.find("opus") != std::string::npos)
    return AudioTrackRecorder::CodecId::OPUS;
  if (codecs_str.find("pcm") != std::string::npos)
    return AudioTrackRecorder::CodecId::PCM;

  return AudioTrackRecorder::CodecId::LAST;
}
