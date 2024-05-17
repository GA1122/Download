DownloadManagerImpl::GetServiceConnector() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  service_manager::Connector* connector = GetServiceManagerConnector();
  if (connector)
    return connector->Clone();   
  return nullptr;
}
