STDMETHODIMP UrlmonUrlRequest::BeginningTransaction(const wchar_t* url,
    const wchar_t* current_headers, DWORD reserved,
    wchar_t** additional_headers) {
  DCHECK_EQ(thread_, base::PlatformThread::CurrentId());
  if (!additional_headers) {
    NOTREACHED();
    return E_POINTER;
  }

  DVLOG(1) << __FUNCTION__ << me() << "headers: \n" << current_headers;

  if (status_.get_state() == Status::ABORTING) {
    DLOG(WARNING) << __FUNCTION__ << me()
                  << ": Aborting connection to URL:"
                  << url
                  << " as the binding has been aborted";
    return E_ABORT;
  }

  HRESULT hr = S_OK;

  std::string new_headers;
  if (post_data_len() > 0) {
    if (is_chunked_upload()) {
      new_headers = base::StringPrintf("Transfer-Encoding: chunked\r\n");
    }
  }

  if (!extra_headers().empty()) {
    new_headers += extra_headers();
  }

  if (!referrer().empty()) {
    new_headers += base::StringPrintf("Referer: %s\r\n", referrer().c_str());
  }

  std::string user_agent = http_utils::AddChromeFrameToUserAgentValue(
      http_utils::GetChromeUserAgent());
  new_headers += ReplaceOrAddUserAgent(current_headers, user_agent);

  if (!new_headers.empty()) {
    *additional_headers = reinterpret_cast<wchar_t*>(
        CoTaskMemAlloc((new_headers.size() + 1) * sizeof(wchar_t)));

    if (*additional_headers == NULL) {
      NOTREACHED();
      hr = E_OUTOFMEMORY;
    } else {
      lstrcpynW(*additional_headers, ASCIIToWide(new_headers).c_str(),
                new_headers.size());
    }
  }
  request_headers_ = new_headers;
  return hr;
}
