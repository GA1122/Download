 static void CopyElementsImpl(FixedArrayBase* from, uint32_t from_start,
 FixedArrayBase* to, ElementsKind from_kind,
 uint32_t to_start, int packed_size,
 int copy_size) {
 DisallowHeapAllocation no_gc;
 ElementsKind to_kind = KindTraits::Kind;
 switch (from_kind) {
 case FAST_SMI_ELEMENTS:
 case FAST_HOLEY_SMI_ELEMENTS:
 case FAST_ELEMENTS:
 case FAST_HOLEY_ELEMENTS:
 CopyObjectToObjectElements(from, from_kind, from_start, to, to_kind,
                                   to_start, copy_size);
 break;
 case FAST_DOUBLE_ELEMENTS:
 case FAST_HOLEY_DOUBLE_ELEMENTS: {
 AllowHeapAllocation allow_allocation;
        DCHECK(IsFastObjectElementsKind(to_kind));
 CopyDoubleToObjectElements(from, from_start, to, to_start, copy_size);
 break;
 }
 case DICTIONARY_ELEMENTS:
 CopyDictionaryToObjectElements(from, from_start, to, to_kind, to_start,
                                       copy_size);
 break;
 case FAST_SLOPPY_ARGUMENTS_ELEMENTS:
 case SLOW_SLOPPY_ARGUMENTS_ELEMENTS:
 case FAST_STRING_WRAPPER_ELEMENTS:
 case SLOW_STRING_WRAPPER_ELEMENTS:
#define TYPED_ARRAY_CASE(Type, type, TYPE, ctype, size) case TYPE##_ELEMENTS:
      TYPED_ARRAYS(TYPED_ARRAY_CASE)
#undef TYPED_ARRAY_CASE
      UNREACHABLE();
 break;
 case NO_ELEMENTS:
 break;  
 }
 }
