void GDataFileSystem::AddObserver(
    GDataFileSystemInterface::Observer* observer) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  observers_.AddObserver(observer);
}
