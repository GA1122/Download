 bool MediaRecorderHandler::CanSupportMimeType(
    const blink::WebString& web_type,
    const blink::WebString& web_codecs) {
  DCHECK(main_render_thread_checker_.CalledOnValidThread());
  if (web_type.IsEmpty())
    return true;

  const std::string type(web_type.Utf8());
  const bool video = base::EqualsCaseInsensitiveASCII(type, "video/webm") ||
                     base::EqualsCaseInsensitiveASCII(type, "video/x-matroska");
  const bool audio =
      video ? false : (base::EqualsCaseInsensitiveASCII(type, "audio/webm"));
  if (!video && !audio)
    return false;

  static const char* const kVideoCodecs[] = {"vp8",  "vp9",  "h264",
                                             "avc1", "opus", "pcm"};
  static const char* const kAudioCodecs[] = {"opus", "pcm"};
  const char* const* codecs = video ? &kVideoCodecs[0] : &kAudioCodecs[0];
  const int codecs_count =
      video ? arraysize(kVideoCodecs) : arraysize(kAudioCodecs);

  std::vector<std::string> codecs_list;
  media::SplitCodecs(web_codecs.Utf8(), &codecs_list);
  media::StripCodecs(&codecs_list);
  for (const auto& codec : codecs_list) {
    auto* const* found = std::find_if(
        &codecs[0], &codecs[codecs_count], [&codec](const char* name) {
          return base::EqualsCaseInsensitiveASCII(codec, name);
        });
    if (found == &codecs[codecs_count])
      return false;
  }
  return true;
}
