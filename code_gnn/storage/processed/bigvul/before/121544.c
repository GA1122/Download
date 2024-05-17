void DriveFileStreamReader::OnGetContent(google_apis::GDataErrorCode error_code,
                                         scoped_ptr<std::string> data) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  DCHECK(reader_proxy_);
  reader_proxy_->OnGetContent(data.Pass());
}
