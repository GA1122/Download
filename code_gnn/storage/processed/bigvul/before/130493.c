bool QueryManager::EndQuery(Query* query, base::subtle::Atomic32 submit_count) {
  DCHECK(query);
  if (!RemovePendingQuery(query)) {
    return false;
  }
  return query->End(submit_count);
}
