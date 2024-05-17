HRESULT CGaiaCredentialBase::GetInstallDirectory(base::FilePath* path) {
  DCHECK(path);

  if (!base::PathService::Get(base::FILE_MODULE, path)) {
    HRESULT hr = HRESULT_FROM_WIN32(::GetLastError());
    LOGFN(ERROR) << "Get(FILE_MODULE) hr=" << putHR(hr);
    return hr;
  }

  *path = path->DirName();
  return S_OK;
}
