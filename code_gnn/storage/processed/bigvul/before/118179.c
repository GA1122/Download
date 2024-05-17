bool ValidityMessages::HasSureError(ServerFieldType field) const {
  return IsSureError(GetMessageOrDefault(field));
}
