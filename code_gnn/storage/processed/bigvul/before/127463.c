const std::string MessageIterator::NextString() const {
  std::string val;
  if (!iter_.ReadString(&val))
    NOTREACHED();
  return val;
}
