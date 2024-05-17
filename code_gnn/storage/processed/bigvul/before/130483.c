void CommandsIssuedQuery::Destroy(bool  ) {
  if (!IsDeleted()) {
    MarkAsDeleted();
  }
}
