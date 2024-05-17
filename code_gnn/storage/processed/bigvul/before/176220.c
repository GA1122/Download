static void CopyDictionaryToObjectElements(
 FixedArrayBase* from_base, uint32_t from_start, FixedArrayBase* to_base,
 ElementsKind to_kind, uint32_t to_start, int raw_copy_size) {
 DisallowHeapAllocation no_allocation;
 SeededNumberDictionary* from = SeededNumberDictionary::cast(from_base);
 int copy_size = raw_copy_size;
 if (raw_copy_size < 0) {
    DCHECK(raw_copy_size == ElementsAccessor::kCopyToEnd ||
           raw_copy_size == ElementsAccessor::kCopyToEndAndInitializeToHole);
    copy_size = from->max_number_key() + 1 - from_start;
 if (raw_copy_size == ElementsAccessor::kCopyToEndAndInitializeToHole) {
 int start = to_start + copy_size;
 int length = to_base->length() - start;
 if (length > 0) {
 Heap* heap = from->GetHeap();
 MemsetPointer(FixedArray::cast(to_base)->data_start() + start,
                      heap->the_hole_value(), length);
 }
 }
 }
  DCHECK(to_base != from_base);
  DCHECK(IsFastSmiOrObjectElementsKind(to_kind));
 if (copy_size == 0) return;
 FixedArray* to = FixedArray::cast(to_base);
 uint32_t to_length = to->length();
 if (to_start + copy_size > to_length) {
    copy_size = to_length - to_start;
 }
 WriteBarrierMode write_barrier_mode = IsFastObjectElementsKind(to_kind)
 ? UPDATE_WRITE_BARRIER
 : SKIP_WRITE_BARRIER;
 Isolate* isolate = from->GetIsolate();
 for (int i = 0; i < copy_size; i++) {
 int entry = from->FindEntry(isolate, i + from_start);
 if (entry != SeededNumberDictionary::kNotFound) {
 Object* value = from->ValueAt(entry);
      DCHECK(!value->IsTheHole(isolate));
      to->set(i + to_start, value, write_barrier_mode);
 } else {
      to->set_the_hole(isolate, i + to_start);
 }
 }
}
