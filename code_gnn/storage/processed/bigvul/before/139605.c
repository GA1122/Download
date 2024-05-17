void MimeUtil::RemoveProprietaryMediaTypesAndCodecs() {
  for (const auto& container : proprietary_media_containers_)
    media_format_map_.erase(container);
  allow_proprietary_codecs_ = false;
}
