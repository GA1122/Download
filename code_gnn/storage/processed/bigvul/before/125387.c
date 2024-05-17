void GDataFileSystem::NotifyFileSystemToBeUnmounted() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  DVLOG(1) << "File System is to be unmounted";
  FOR_EACH_OBSERVER(GDataFileSystemInterface::Observer, observers_,
                    OnFileSystemBeingUnmounted());
}
