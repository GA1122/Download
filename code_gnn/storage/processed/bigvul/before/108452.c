void ChromeRenderProcessObserver::OnGetCacheResourceStats() {
  WebCache::ResourceTypeStats stats;
  WebCache::getResourceTypeStats(&stats);
   RenderThread::Get()->Send(new ChromeViewHostMsg_ResourceTypeStats(stats));
 }
