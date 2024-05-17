void Job::Start(Operation op,
                bool blocking_dns,
                const CompletionCallback& callback) {
  CheckIsOnOriginThread();

  operation_ = op;
  blocking_dns_ = blocking_dns;
  SetCallback(callback);

  owned_self_reference_ = this;

  worker_task_runner()->PostTask(
      FROM_HERE, blocking_dns_ ? base::Bind(&Job::ExecuteBlocking, this)
                               : base::Bind(&Job::ExecuteNonBlocking, this));
}
