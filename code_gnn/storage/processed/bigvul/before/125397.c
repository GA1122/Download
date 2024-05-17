void GDataFileSystem::OnFeedFromServerLoaded() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  FOR_EACH_OBSERVER(GDataFileSystemInterface::Observer, observers_,
                    OnFeedFromServerLoaded());
}
