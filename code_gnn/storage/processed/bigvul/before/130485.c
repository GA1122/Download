void AsyncReadPixelsCompletedQuery::Destroy(bool  ) {
  if (!IsDeleted()) {
    MarkAsDeleted();
  }
}
