void DOMStorageContextWrapper::GetSessionStorageUsage(
    GetSessionStorageUsageCallback callback) {
  if (mojo_session_state_) {
    mojo_task_runner_->PostTask(
        FROM_HERE,
        base::BindOnce(&SessionStorageContextMojo::GetStorageUsage,
                       base::Unretained(mojo_session_state_),
                       base::BindOnce(&GotMojoSessionStorageUsage,
                                      base::ThreadTaskRunnerHandle::Get(),
                                      std::move(callback))));
    return;
  }
  DCHECK(context_.get());
  context_->task_runner()->PostShutdownBlockingTask(
      FROM_HERE, DOMStorageTaskRunner::PRIMARY_SEQUENCE,
      base::BindOnce(&GetSessionStorageUsageHelper,
                     base::RetainedRef(base::ThreadTaskRunnerHandle::Get()),
                     base::RetainedRef(context_), std::move(callback)));
}
