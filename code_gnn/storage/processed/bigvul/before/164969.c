void ResourceDispatcherHostImpl::Shutdown() {
  DCHECK(main_thread_task_runner_->BelongsToCurrentThread());
  io_thread_task_runner_->PostTask(
      FROM_HERE, base::BindOnce(&ResourceDispatcherHostImpl::OnShutdown,
                                base::Unretained(this)));
}
