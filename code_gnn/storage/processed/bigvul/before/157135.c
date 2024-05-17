bool UrlData::FullyCached() {
  DCHECK(thread_checker_.CalledOnValidThread());
  if (length_ == kPositionNotSpecified)
    return false;
  return (multibuffer()->FindNextUnavailable(0) << kBlockSizeShift) >= length_;
}
