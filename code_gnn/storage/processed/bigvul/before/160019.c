void BackendImpl::DecreaseNumRefs() {
  DCHECK(num_refs_);
  num_refs_--;

  if (!num_refs_ && disabled_)
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE, base::Bind(&BackendImpl::RestartCache, GetWeakPtr(), true));
}
