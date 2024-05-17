void RenderProcessHostImpl::RegisterAecDumpConsumerOnUIThread(int id) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  aec_dump_consumers_.push_back(id);

  if (WebRTCInternals::GetInstance()->IsAudioDebugRecordingsEnabled()) {
    base::FilePath file_with_extensions = GetAecDumpFilePathWithExtensions(
        WebRTCInternals::GetInstance()->GetAudioDebugRecordingsFilePath());
    EnableAecDumpForId(file_with_extensions, id);
  }
}
