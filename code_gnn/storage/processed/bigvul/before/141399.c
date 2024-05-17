    DelayScrollOffsetClampScope() {
  DCHECK(count_ > 0 || NeedsClampList().IsEmpty());
  count_++;
}
