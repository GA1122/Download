void DOMStorageContextWrapper::PurgeMemory(DOMStorageContextImpl::PurgeOption
    purge_option) {
  context_->task_runner()->PostTask(
      FROM_HERE, base::BindOnce(&DOMStorageContextImpl::PurgeMemory, context_,
                                purge_option));
  if (mojo_state_ && purge_option == DOMStorageContextImpl::PURGE_AGGRESSIVE) {
    mojo_task_runner_->PostTask(
        FROM_HERE, base::BindOnce(&LocalStorageContextMojo::PurgeMemory,
                                  base::Unretained(mojo_state_)));
  }

  if (mojo_session_state_ &&
      purge_option == DOMStorageContextImpl::PURGE_AGGRESSIVE) {
    mojo_task_runner_->PostTask(
        FROM_HERE, base::BindOnce(&SessionStorageContextMojo::PurgeMemory,
                                  base::Unretained(mojo_session_state_)));
  }
}
