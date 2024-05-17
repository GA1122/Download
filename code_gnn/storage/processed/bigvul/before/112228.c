QuotaTaskObserver::~QuotaTaskObserver() {
  std::for_each(running_quota_tasks_.begin(),
                running_quota_tasks_.end(),
                std::mem_fun(&QuotaTask::Abort));
}
