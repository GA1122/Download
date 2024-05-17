void LargeObjectPage::checkAndMarkPointer(Visitor* visitor, Address address) {
#if DCHECK_IS_ON()
  DCHECK(contains(address));
#endif
  if (!containedInObjectPayload(address))
    return;
  markPointer(visitor, heapObjectHeader());
}
