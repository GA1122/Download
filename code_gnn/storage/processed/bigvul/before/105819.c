STDMETHODIMP UrlmonUrlRequest::GetBindInfo(DWORD* bind_flags,
                                           BINDINFO* bind_info) {
  if ((bind_info == NULL) || (bind_info->cbSize == 0) || (bind_flags == NULL))
    return E_INVALIDARG;

  *bind_flags = BINDF_ASYNCHRONOUS | BINDF_ASYNCSTORAGE | BINDF_PULLDATA;

  bind_info->dwOptionsFlags = INTERNET_FLAG_NO_AUTO_REDIRECT;
  bind_info->dwOptions = BINDINFO_OPTIONS_WININETFLAG;

  if (load_flags_ & net::LOAD_VALIDATE_CACHE)
    *bind_flags |= BINDF_RESYNCHRONIZE;

  if (load_flags_ & net::LOAD_BYPASS_CACHE)
    *bind_flags |= BINDF_GETNEWESTVERSION;

  if (LowerCaseEqualsASCII(method(), "get")) {
    bind_info->dwBindVerb = BINDVERB_GET;
  } else if (LowerCaseEqualsASCII(method(), "post")) {
    bind_info->dwBindVerb = BINDVERB_POST;
  } else if (LowerCaseEqualsASCII(method(), "put")) {
    bind_info->dwBindVerb = BINDVERB_PUT;
  } else {
    std::wstring verb(ASCIIToWide(StringToUpperASCII(method())));
    bind_info->dwBindVerb = BINDVERB_CUSTOM;
    bind_info->szCustomVerb = reinterpret_cast<wchar_t*>(
        ::CoTaskMemAlloc((verb.length() + 1) * sizeof(wchar_t)));
    lstrcpyW(bind_info->szCustomVerb, verb.c_str());
  }

  if (post_data_len()) {
    *bind_flags |= BINDF_GETNEWESTVERSION | BINDF_PRAGMA_NO_CACHE;

    if (resource_type_ != ResourceType::MAIN_FRAME)
      *bind_flags |= BINDF_NOWRITECACHE;

    memset(&bind_info->stgmedData, 0, sizeof(STGMEDIUM));
    bind_info->grfBindInfoF = 0;

    if (bind_info->dwBindVerb != BINDVERB_CUSTOM)
      bind_info->szCustomVerb = NULL;

    if (get_upload_data(&bind_info->stgmedData.pstm) == S_OK) {
      bind_info->stgmedData.tymed = TYMED_ISTREAM;
#pragma warning(disable:4244)
      bind_info->cbstgmedData = post_data_len();
#pragma warning(default:4244)
      DVLOG(1) << __FUNCTION__ << me() << method()
               << " request with " << base::Int64ToString(post_data_len())
               << " bytes. url=" << url();
    } else {
      DVLOG(1) << __FUNCTION__ << me() << "POST request with no data!";
    }
  }

  return S_OK;
}
