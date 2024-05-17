void GDataFileSystem::OnDocumentFeedFetched(int num_accumulated_entries) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  FOR_EACH_OBSERVER(GDataFileSystemInterface::Observer, observers_,
                    OnDocumentFeedFetched(num_accumulated_entries));
}
