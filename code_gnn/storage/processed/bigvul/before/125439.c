void GDataFileSystem::RemoveObserver(
    GDataFileSystemInterface::Observer* observer) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  observers_.RemoveObserver(observer);
}
