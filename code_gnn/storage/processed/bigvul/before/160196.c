MemBackendImpl::~MemBackendImpl() {
  DCHECK(CheckLRUListOrder(lru_list_));
  while (!entries_.empty())
    entries_.begin()->second->Doom();

  if (!post_cleanup_callback_.is_null())
    base::SequencedTaskRunnerHandle::Get()->PostTask(
        FROM_HERE, std::move(post_cleanup_callback_));
}
