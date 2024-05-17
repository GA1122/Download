void GDataFileSystem::OnDirectoryChanged(const FilePath& directory_path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  FOR_EACH_OBSERVER(GDataFileSystemInterface::Observer, observers_,
                    OnDirectoryChanged(directory_path));
}
