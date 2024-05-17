ULONG DataObjectImpl::AddRef() {
  base::RefCountedThreadSafe<DownloadFileObserver>::AddRef();
  return 0;
}
