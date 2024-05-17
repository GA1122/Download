void MarkingVisitor::ConservativelyMarkAddress(BasePage* page,
                                               Address address) {
#if DCHECK_IS_ON()
  DCHECK(page->Contains(address));
#endif
  HeapObjectHeader* const header =
      page->IsLargeObjectPage()
          ? static_cast<LargeObjectPage*>(page)->GetHeapObjectHeader()
          : static_cast<NormalPage*>(page)->FindHeaderFromAddress(address);
  if (!header)
    return;
  ConservativelyMarkHeader(header);
}
