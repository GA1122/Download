bool ValidityMessages::HasSureErrors() const {
 for (MessageMap::const_iterator iter = messages_.begin();
      iter != messages_.end(); ++iter) {
    if (IsSureError(iter->second))
      return true;
  }
  return false;
}
