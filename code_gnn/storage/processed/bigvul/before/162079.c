void RenderProcessHostImpl::EnableAudioDebugRecordings(
    const base::FilePath& file) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  base::FilePath file_with_extensions = GetAecDumpFilePathWithExtensions(file);
  for (int id : aec_dump_consumers_) {
    EnableAecDumpForId(file_with_extensions, id);
  }
}
