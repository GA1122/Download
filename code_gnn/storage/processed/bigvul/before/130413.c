void ThreadWatcher::OnPingMessage(const BrowserThread::ID& thread_id,
                                  const base::Closure& callback_task) {
  DCHECK(BrowserThread::CurrentlyOn(thread_id));
  WatchDogThread::PostTask(FROM_HERE, callback_task);
}
