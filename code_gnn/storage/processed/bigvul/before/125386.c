void GDataFileSystem::NotifyFileSystemMounted() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  DVLOG(1) << "File System is mounted";
  FOR_EACH_OBSERVER(GDataFileSystemInterface::Observer, observers_,
                    OnFileSystemMounted());
}
