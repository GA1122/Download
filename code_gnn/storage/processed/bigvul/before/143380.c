void Job::NotifyCaller(int result) {
  CheckIsOnWorkerThread();

  origin_runner_->PostTask(
      FROM_HERE, base::Bind(&Job::NotifyCallerOnOriginLoop, this, result));
}
