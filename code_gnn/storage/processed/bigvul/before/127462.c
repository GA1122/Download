int MessageIterator::NextInt() const {
  int val = -1;
  if (!iter_.ReadInt(&val))
    NOTREACHED();
  return val;
}
