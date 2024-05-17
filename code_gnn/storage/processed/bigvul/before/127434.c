void FileAPIMessageFilter::OnCancel(
    int request_id,
    int request_id_to_cancel) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  FileSystemOperation* write = operations_.Lookup(request_id_to_cancel);
  if (write) {
    write->Cancel(
        base::Bind(&FileAPIMessageFilter::DidCancel, this, request_id));
  } else {
    Send(new FileSystemMsg_DidFail(
        request_id, base::PLATFORM_FILE_ERROR_INVALID_OPERATION));
  }
}
