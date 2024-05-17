void GLES2Implementation::SetActiveURLCHROMIUM(const char* url) {
  DCHECK(url);
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glSetActiveURLCHROMIUM(" << url);

  if (last_active_url_ == url)
    return;

  last_active_url_ = url;
  static constexpr uint32_t kMaxStrLen = 1024;
  size_t len = strlen(url);
  if (len == 0)
    return;

  SetBucketContents(kResultBucketId, url,
                    base::CheckMin(len, kMaxStrLen).ValueOrDie());
  helper_->SetActiveURLCHROMIUM(kResultBucketId);
  helper_->SetBucketSize(kResultBucketId, 0);
}
