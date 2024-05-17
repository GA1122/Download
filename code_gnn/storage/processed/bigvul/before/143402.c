const scoped_refptr<base::SingleThreadTaskRunner>& Job::worker_task_runner() {
  return params_->worker_task_runner;
}
