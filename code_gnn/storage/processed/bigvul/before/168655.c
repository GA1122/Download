void IndexedDBTransaction::RunTasksIfStarted() {
  DCHECK(used_);

  if (state_ != STARTED)
    return;

  if (should_process_queue_)
    return;

  should_process_queue_ = true;
  base::SequencedTaskRunnerHandle::Get()->PostTask(
      FROM_HERE, base::BindOnce(&IndexedDBTransaction::ProcessTaskQueue,
                                ptr_factory_.GetWeakPtr()));
}
