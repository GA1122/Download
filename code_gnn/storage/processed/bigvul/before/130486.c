void GetErrorQuery::Destroy(bool  ) {
  if (!IsDeleted()) {
    MarkAsDeleted();
  }
}
