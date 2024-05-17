bool QueryManager::Query::MarkAsCompleted(uint64 result) {
  DCHECK(pending_);
  QuerySync* sync = manager_->decoder_->GetSharedMemoryAs<QuerySync*>(
      shm_id_, shm_offset_, sizeof(*sync));
  if (!sync) {
    return false;
  }

  pending_ = false;
  sync->result = result;
  base::subtle::Release_Store(&sync->process_count, submit_count_);

  return true;
}
