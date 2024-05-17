 void CopyElements(Handle<FixedArrayBase> source, ElementsKind source_kind,
 Handle<FixedArrayBase> destination, int size) {
 Subclass::CopyElementsImpl(*source, 0, *destination, source_kind, 0,
                               kPackedSizeNotKnown, size);
 }
