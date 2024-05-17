void DOMStorageContextWrapper::PerformLocalStorageCleanup(
    base::OnceClosure callback) {
  DCHECK(context_.get());
  DCHECK(callback);
  mojo_task_runner_->PostTask(
      FROM_HERE,
      base::BindOnce(
          &LocalStorageContextMojo::PerformStorageCleanup,
          base::Unretained(mojo_state_),
          base::BindOnce(&GotMojoCallback, base::ThreadTaskRunnerHandle::Get(),
                         std::move(callback))));
}
