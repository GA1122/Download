STDMETHODIMP UrlmonUrlRequest::OnDataAvailable(DWORD flags, DWORD size,
                                               FORMATETC* formatetc,
                                               STGMEDIUM* storage) {
  DCHECK_EQ(thread_, base::PlatformThread::CurrentId());
  DVLOG(1) << __FUNCTION__ << me() << "bytes available: " << size;

  if (terminate_requested()) {
    DVLOG(1) << " Download requested. INET_E_TERMINATED_BIND returned";
    return INET_E_TERMINATED_BIND;
  }

  if (!storage || (storage->tymed != TYMED_ISTREAM)) {
    NOTREACHED();
    return E_INVALIDARG;
  }

  IStream* read_stream = storage->pstm;
  if (!read_stream) {
    NOTREACHED();
    return E_UNEXPECTED;
  }

  if (size > 0)
    pending_data_ = read_stream;

  if (pending_read_size_) {
    size_t bytes_copied = SendDataToDelegate(pending_read_size_);
    DVLOG(1) << __FUNCTION__ << me() << "size read: " << bytes_copied;
  } else {
    DVLOG(1) << __FUNCTION__ << me() << "- waiting for remote read";
  }

  if (BSCF_LASTDATANOTIFICATION & flags) {
    if (!is_expecting_download_ || pending()) {
      DVLOG(1) << __FUNCTION__ << me() << "EOF";
      return S_OK;
    }
    DVLOG(1) << __FUNCTION__ << " EOF: INET_E_TERMINATED_BIND returned";
    return INET_E_TERMINATED_BIND;
  }
  return S_OK;
}
