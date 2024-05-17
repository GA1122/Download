void WebMediaPlayerImpl::RecordUnderflowDuration(base::TimeDelta duration) {
  DCHECK(data_source_ || chunk_demuxer_);

  if (data_source_)
    UMA_HISTOGRAM_TIMES("Media.UnderflowDuration2.SRC", duration);
  else
    UMA_HISTOGRAM_TIMES("Media.UnderflowDuration2.MSE", duration);

  if (is_encrypted_)
    UMA_HISTOGRAM_TIMES("Media.UnderflowDuration2.EME", duration);
}
