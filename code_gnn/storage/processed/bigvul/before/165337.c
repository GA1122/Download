 void DOMStorageContextWrapper::Flush() {
  DCHECK(context_.get());

  context_->task_runner()->PostShutdownBlockingTask(
      FROM_HERE, DOMStorageTaskRunner::PRIMARY_SEQUENCE,
      base::BindOnce(&DOMStorageContextImpl::Flush, context_));
  mojo_task_runner_->PostTask(FROM_HERE,
                              base::BindOnce(&LocalStorageContextMojo::Flush,
                                             base::Unretained(mojo_state_)));
  if (mojo_session_state_) {
    mojo_task_runner_->PostTask(
        FROM_HERE, base::BindOnce(&SessionStorageContextMojo::Flush,
                                  base::Unretained(mojo_session_state_)));
  }
}
