void UrlmonUrlRequest::TerminateBind(TerminateBindCallback* callback) {
  DCHECK_EQ(thread_, base::PlatformThread::CurrentId());
  DVLOG(1) << __FUNCTION__ << me();
  cleanup_transaction_ = false;
  if (status_.get_state() == Status::DONE) {
    callback->Run(moniker_, bind_context_, upload_data_,
                  request_headers_.c_str());
    delete callback;
  } else {
    terminate_bind_callback_.reset(callback);
    if (pending_data_) {
      std::string data;
      base::win::ScopedComPtr<IStream> read_stream(pending_data_);
      HRESULT hr;
      while ((hr = ReadStream(read_stream, 0xffff, &data)) == S_OK) {
      }
      DLOG_IF(WARNING, hr != E_PENDING) << __FUNCTION__ <<
          base::StringPrintf(" expected E_PENDING but got 0x%08X", hr);
    }
  }
}
