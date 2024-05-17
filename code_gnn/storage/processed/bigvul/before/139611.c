bool IsDolbyVisionAVCCodecId(const std::string& codec_id) {
  return base::StartsWith(codec_id, "dva1.", base::CompareCase::SENSITIVE) ||
         base::StartsWith(codec_id, "dvav.", base::CompareCase::SENSITIVE);
}
