void StoragePartitionImpl::DataDeletionHelper::DecrementTaskCount() {
  if (!BrowserThread::CurrentlyOn(BrowserThread::UI)) {
    base::PostTaskWithTraits(
        FROM_HERE, {BrowserThread::UI},
        base::BindOnce(&DataDeletionHelper::DecrementTaskCount,
                       base::Unretained(this)));
    return;
  }
  DCHECK_GT(task_count_, 0);
  --task_count_;
  if (!task_count_) {
    std::move(callback_).Run();
    delete this;
  }
}
