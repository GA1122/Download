void WebMediaPlayerImpl::RecordTimingUMA(const std::string& key,
                                         base::TimeDelta elapsed) {
  if (chunk_demuxer_)
    base::UmaHistogramMediumTimes(key + ".MSE", elapsed);
  else
    base::UmaHistogramMediumTimes(key + ".SRC", elapsed);
  if (is_encrypted_)
    base::UmaHistogramMediumTimes(key + ".EME", elapsed);
}
