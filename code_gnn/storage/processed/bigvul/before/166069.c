void FileSystemManagerImpl::DidWrite(OperationListenerID listener_id,
                                     base::File::Error result,
                                     int64_t bytes,
                                     bool complete) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  blink::mojom::FileSystemOperationListener* listener =
      GetOpListener(listener_id);
  if (!listener)
    return;
  if (result == base::File::FILE_OK) {
    listener->DidWrite(bytes, complete);
    if (complete)
      RemoveOpListener(listener_id);
  } else {
    listener->ErrorOccurred(result);
    RemoveOpListener(listener_id);
  }
}
