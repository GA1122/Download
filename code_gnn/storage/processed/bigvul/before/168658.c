void IndexedDBTransaction::Start() {
  DCHECK_EQ(CREATED, state_);
  state_ = STARTED;
  diagnostics_.start_time = base::Time::Now();

  if (!used_) {
    if (commit_pending_) {
      base::SequencedTaskRunnerHandle::Get()->PostTask(
          FROM_HERE, base::BindOnce(&CommitUnused, ptr_factory_.GetWeakPtr()));
    }
    return;
  }

  RunTasksIfStarted();
}
