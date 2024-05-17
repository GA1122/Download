 void WebFrame::startDownload(const WebCore::ResourceRequest& request)
  {
      ASSERT(m_policyDownloadID);
  
    DownloadManager::shared().startDownload(m_policyDownloadID, request);
//     DownloadManager::shared().startDownload(m_policyDownloadID, page(), request);
  
      m_policyDownloadID = 0;
  }