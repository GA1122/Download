void GLES2DecoderImpl::ReportProgress() {
  if (group_)
    group_->ReportProgress();
}
