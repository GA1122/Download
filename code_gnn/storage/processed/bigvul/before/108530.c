void QuotaManager::PostTaskAndReplyWithResultForDBThread(
    const tracked_objects::Location& from_here,
    const base::Callback<bool(QuotaDatabase*)>& task,
    const base::Callback<void(bool)>& reply) {
  base::PostTaskAndReplyWithResult(
      db_thread_,
      from_here,
      base::Bind(task, base::Unretained(database_.get())),
      reply);
}
