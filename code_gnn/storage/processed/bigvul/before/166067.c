void FileSystemManagerImpl::DidReceiveSnapshotFile(int snapshot_id) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  in_transit_snapshot_files_.Remove(snapshot_id);
}
