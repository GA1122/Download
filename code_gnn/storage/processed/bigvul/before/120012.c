  void CompleteGetFileProperties(drive::FileError error) {
    DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
    DCHECK(!callback_.is_null());
    callback_.Run(error);

    delete this;
  }
