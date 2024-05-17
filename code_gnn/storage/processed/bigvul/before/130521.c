QueryManager::~QueryManager() {
  DCHECK(queries_.empty());

  CHECK_EQ(query_count_, 0u);
}
