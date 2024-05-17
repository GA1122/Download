void MarkingVisitor::ConservativelyMarkAddress(
    BasePage* page,
    Address address,
    MarkedPointerCallbackForTesting callback) {
  DCHECK(page->Contains(address));
  HeapObjectHeader* const header =
      page->IsLargeObjectPage()
          ? static_cast<LargeObjectPage*>(page)->GetHeapObjectHeader()
          : static_cast<NormalPage*>(page)->FindHeaderFromAddress(address);
  if (!header)
    return;
  if (!callback(header))
    ConservativelyMarkHeader(header);
}
