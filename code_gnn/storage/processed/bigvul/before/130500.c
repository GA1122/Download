bool AsyncPixelTransfersCompletedQuery::Process() {
  QuerySync* sync = manager()->decoder()->GetSharedMemoryAs<QuerySync*>(
      shm_id(), shm_offset(), sizeof(*sync));
  if (!sync)
    return false;

  if (base::subtle::Acquire_Load(&sync->process_count) != submit_count())
    return true;

  UnmarkAsPending();
  return true;
}
