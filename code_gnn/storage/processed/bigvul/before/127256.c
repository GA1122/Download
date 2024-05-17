void WtsSessionProcessDelegate::Core::InitializeJob() {
  DCHECK(io_task_runner_->BelongsToCurrentThread());

  ScopedHandle job;
  job.Set(CreateJobObject(NULL, NULL));
  if (!job.IsValid()) {
    LOG_GETLASTERROR(ERROR) << "Failed to create a job object";
    return;
  }

  JOBOBJECT_EXTENDED_LIMIT_INFORMATION info;
  memset(&info, 0, sizeof(info));
  info.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_ACTIVE_PROCESS |
      JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
  info.BasicLimitInformation.ActiveProcessLimit = 2;
  if (!SetInformationJobObject(job,
                               JobObjectExtendedLimitInformation,
                               &info,
                               sizeof(info))) {
    LOG_GETLASTERROR(ERROR) << "Failed to set limits on the job object";
    return;
  }

  if (!MessageLoopForIO::current()->RegisterJobObject(job, this)) {
    LOG_GETLASTERROR(ERROR)
        << "Failed to associate the job object with a completion port";
    return;
  }

  scoped_ptr<ScopedHandle> job_wrapper(new ScopedHandle());
  *job_wrapper = job.Pass();

  main_task_runner_->PostTask(FROM_HERE, base::Bind(
      &Core::InitializeJobCompleted, this, base::Passed(&job_wrapper)));
}
