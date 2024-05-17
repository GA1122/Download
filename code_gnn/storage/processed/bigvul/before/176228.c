 static void CopyElementsImpl(FixedArrayBase* from, uint32_t from_start,
 FixedArrayBase* to, ElementsKind from_kind,
 uint32_t to_start, int packed_size,
 int copy_size) {
    DCHECK(!to->IsDictionary());
 if (from_kind == SLOW_STRING_WRAPPER_ELEMENTS) {
 CopyDictionaryToObjectElements(from, from_start, to, FAST_HOLEY_ELEMENTS,
                                     to_start, copy_size);
 } else {
      DCHECK_EQ(FAST_STRING_WRAPPER_ELEMENTS, from_kind);
 CopyObjectToObjectElements(from, FAST_HOLEY_ELEMENTS, from_start, to,
                                 FAST_HOLEY_ELEMENTS, to_start, copy_size);
 }
 }
