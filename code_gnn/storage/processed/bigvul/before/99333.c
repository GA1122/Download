void ResourceMessageFilter::OnResourceTypeStatsOnUIThread(
    WebCache::ResourceTypeStats stats, base::ProcessId renderer_id) {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::UI));
  TaskManager::GetInstance()->model()->NotifyResourceTypeStats(
      renderer_id, stats);
}
