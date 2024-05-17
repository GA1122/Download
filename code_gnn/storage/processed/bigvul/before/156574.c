void SessionStore::WriteBatch::Commit(std::unique_ptr<WriteBatch> batch) {
  DCHECK(batch);
  std::move(batch->commit_cb_)
      .Run(std::move(batch->batch_),
           base::BindOnce(&ForwardError, std::move(batch->error_handler_)));
}
