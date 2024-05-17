 static void CopyElementsImpl(FixedArrayBase* from, uint32_t from_start,
 FixedArrayBase* to, ElementsKind from_kind,
 uint32_t to_start, int packed_size,
 int copy_size) {
 DisallowHeapAllocation no_allocation;
 switch (from_kind) {
 case FAST_SMI_ELEMENTS:
 CopyPackedSmiToDoubleElements(from, from_start, to, to_start,
                                      packed_size, copy_size);
 break;
 case FAST_HOLEY_SMI_ELEMENTS:
 CopySmiToDoubleElements(from, from_start, to, to_start, copy_size);
 break;
 case FAST_DOUBLE_ELEMENTS:
 case FAST_HOLEY_DOUBLE_ELEMENTS:
 CopyDoubleToDoubleElements(from, from_start, to, to_start, copy_size);
 break;
 case FAST_ELEMENTS:
 case FAST_HOLEY_ELEMENTS:
 CopyObjectToDoubleElements(from, from_start, to, to_start, copy_size);
 break;
 case DICTIONARY_ELEMENTS:
 CopyDictionaryToDoubleElements(from, from_start, to, to_start,
                                       copy_size);
 break;
 case FAST_SLOPPY_ARGUMENTS_ELEMENTS:
 case SLOW_SLOPPY_ARGUMENTS_ELEMENTS:
 case FAST_STRING_WRAPPER_ELEMENTS:
 case SLOW_STRING_WRAPPER_ELEMENTS:
 case NO_ELEMENTS:
#define TYPED_ARRAY_CASE(Type, type, TYPE, ctype, size) case TYPE##_ELEMENTS:
      TYPED_ARRAYS(TYPED_ARRAY_CASE)
#undef TYPED_ARRAY_CASE
      UNREACHABLE();
 break;
 }
 }
