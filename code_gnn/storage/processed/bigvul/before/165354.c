void DOMStorageContextWrapper::StartScavengingUnusedSessionStorage() {
  if (mojo_session_state_) {
    mojo_task_runner_->PostTask(
        FROM_HERE,
        base::BindOnce(&SessionStorageContextMojo::ScavengeUnusedNamespaces,
                       base::Unretained(mojo_session_state_),
                       base::OnceClosure()));
    return;
  }
  DCHECK(context_.get());
  context_->task_runner()->PostShutdownBlockingTask(
      FROM_HERE, DOMStorageTaskRunner::PRIMARY_SEQUENCE,
      base::BindOnce(
          &DOMStorageContextImpl::StartScavengingUnusedSessionStorage,
          context_));
}
