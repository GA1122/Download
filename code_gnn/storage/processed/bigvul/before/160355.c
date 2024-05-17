void NormalPage::checkAndMarkPointer(Visitor* visitor,
                                     Address address,
                                     MarkedPointerCallbackForTesting callback) {
  DCHECK(contains(address));
  HeapObjectHeader* header = findHeaderFromAddress(address);
  if (!header)
    return;
  if (!callback(header))
    markPointer(visitor, header);
}
