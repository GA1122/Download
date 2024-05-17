  bool IsMarked() const {
    return HeapObjectHeader::FromPayload(this)->IsMarked();
  }
