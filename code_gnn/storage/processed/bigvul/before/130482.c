void AllSamplesPassedQuery::Destroy(bool have_context) {
  if (have_context && !IsDeleted()) {
    glDeleteQueriesARB(1, &service_id_);
    MarkAsDeleted();
  }
}
