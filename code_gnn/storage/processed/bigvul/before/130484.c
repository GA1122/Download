void CommandLatencyQuery::Destroy(bool  ) {
  if (!IsDeleted()) {
    MarkAsDeleted();
  }
}
