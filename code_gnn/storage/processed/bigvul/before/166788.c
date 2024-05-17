void NormalPage::VerifyMarking() {
  DCHECK(!ArenaForNormalPage()
              ->GetThreadState()
              ->Heap()
              .GetStackFrameDepth()
              .IsSafeToRecurse());
  DCHECK(!ArenaForNormalPage()->CurrentAllocationPoint());
  MarkingVerifier verifier(ArenaForNormalPage()->GetThreadState());
  for (Address header_address = Payload(); header_address < PayloadEnd();) {
    HeapObjectHeader* header =
        reinterpret_cast<HeapObjectHeader*>(header_address);
    verifier.VerifyObject(header);
    header_address += header->size();
  }
}
