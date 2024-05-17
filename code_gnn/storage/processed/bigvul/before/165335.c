void DOMStorageContextWrapper::DeleteLocalStorage(const url::Origin& origin,
                                                  base::OnceClosure callback) {
  DCHECK(context_.get());
  DCHECK(callback);
  if (!legacy_localstorage_path_.empty()) {
    context_->task_runner()->PostShutdownBlockingTask(
        FROM_HERE, DOMStorageTaskRunner::PRIMARY_SEQUENCE,
        base::BindOnce(
            base::IgnoreResult(&sql::Database::Delete),
            legacy_localstorage_path_.Append(
                DOMStorageArea::DatabaseFileNameFromOrigin(origin))));
  }
  mojo_task_runner_->PostTask(
      FROM_HERE,
      base::BindOnce(
          &LocalStorageContextMojo::DeleteStorage,
          base::Unretained(mojo_state_), origin,
          base::BindOnce(&GotMojoCallback, base::ThreadTaskRunnerHandle::Get(),
                         std::move(callback))));
}
