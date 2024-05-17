void SyncBackendHost::RefreshNigori(const base::Closure& done_callback) {
  DCHECK_EQ(MessageLoop::current(), frontend_loop_);
  base::Closure sync_thread_done_callback =
      base::Bind(&PostClosure,
                 MessageLoop::current(), FROM_HERE, done_callback);
  sync_thread_.message_loop()->PostTask(
      FROM_HERE,
      base::Bind(&SyncBackendHost::Core::DoRefreshNigori,
                 core_.get(), sync_thread_done_callback));
}
