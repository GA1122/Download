void DOMStorageContextWrapper::DeleteSessionStorage(
    const SessionStorageUsageInfo& usage_info,
    base::OnceClosure callback) {
  if (mojo_session_state_) {
    mojo_task_runner_->PostTask(
        FROM_HERE,
        base::BindOnce(&SessionStorageContextMojo::DeleteStorage,
                       base::Unretained(mojo_session_state_),
                       url::Origin::Create(usage_info.origin),
                       usage_info.namespace_id, std::move(callback)));
    return;
  }
  DCHECK(context_.get());
  context_->task_runner()->PostShutdownBlockingTask(
      FROM_HERE, DOMStorageTaskRunner::PRIMARY_SEQUENCE,
      base::BindOnce(&DOMStorageContextImpl::DeleteSessionStorage, context_,
                     usage_info));
  std::move(callback).Run();
}
