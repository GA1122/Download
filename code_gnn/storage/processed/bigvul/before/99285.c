void ResourceMessageFilter::OnDestruct() {
  ChromeThread::DeleteOnIOThread::Destruct(this);
}
