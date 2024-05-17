void MimeUtil::AddContainerWithCodecs(const std::string& mime_type,
                                      const CodecSet& codecs,
                                      bool is_proprietary_mime_type) {
#if !BUILDFLAG(USE_PROPRIETARY_CODECS)
  DCHECK(!is_proprietary_mime_type);
#endif

  media_format_map_[mime_type] = codecs;

  if (is_proprietary_mime_type)
    proprietary_media_containers_.push_back(mime_type);
}
