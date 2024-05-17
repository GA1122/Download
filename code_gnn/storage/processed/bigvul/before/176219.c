static void CopyDictionaryToDoubleElements(FixedArrayBase* from_base,
 uint32_t from_start,
 FixedArrayBase* to_base,
 uint32_t to_start,
 int raw_copy_size) {
 DisallowHeapAllocation no_allocation;
 SeededNumberDictionary* from = SeededNumberDictionary::cast(from_base);
 int copy_size = raw_copy_size;
 if (copy_size < 0) {
    DCHECK(copy_size == ElementsAccessor::kCopyToEnd ||
           copy_size == ElementsAccessor::kCopyToEndAndInitializeToHole);
    copy_size = from->max_number_key() + 1 - from_start;
 if (raw_copy_size == ElementsAccessor::kCopyToEndAndInitializeToHole) {
 for (int i = to_start + copy_size; i < to_base->length(); ++i) {
 FixedDoubleArray::cast(to_base)->set_the_hole(i);
 }
 }
 }
 if (copy_size == 0) return;
 FixedDoubleArray* to = FixedDoubleArray::cast(to_base);
 uint32_t to_length = to->length();
 if (to_start + copy_size > to_length) {
    copy_size = to_length - to_start;
 }
 Isolate* isolate = from->GetIsolate();
 for (int i = 0; i < copy_size; i++) {
 int entry = from->FindEntry(isolate, i + from_start);
 if (entry != SeededNumberDictionary::kNotFound) {
      to->set(i + to_start, from->ValueAt(entry)->Number());
 } else {
      to->set_the_hole(i + to_start);
 }
 }
}
