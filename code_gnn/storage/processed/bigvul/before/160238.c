  content::ServiceWorkerContext* GetServiceWorkerContext() {
    return content::BrowserContext::GetDefaultStoragePartition(
               browser()->profile())
        ->GetServiceWorkerContext();
  }
