void AsyncPixelTransfersCompletedQuery::Destroy(bool  ) {
  if (!IsDeleted()) {
    MarkAsDeleted();
  }
}
