void LargeObjectPage::checkAndMarkPointer(
    Visitor* visitor,
    Address address,
    MarkedPointerCallbackForTesting callback) {
  DCHECK(contains(address));
  if (!containedInObjectPayload(address))
    return;
  if (!callback(heapObjectHeader()))
    markPointer(visitor, heapObjectHeader());
}
