void CopyObjectToObjectElements(FixedArrayBase* from_base,
 ElementsKind from_kind, uint32_t from_start,
 FixedArrayBase* to_base, ElementsKind to_kind,
 uint32_t to_start, int raw_copy_size) {
  DCHECK(to_base->map() !=
      from_base->GetIsolate()->heap()->fixed_cow_array_map());
 DisallowHeapAllocation no_allocation;
 int copy_size = raw_copy_size;
 if (raw_copy_size < 0) {
    DCHECK(raw_copy_size == ElementsAccessor::kCopyToEnd ||
           raw_copy_size == ElementsAccessor::kCopyToEndAndInitializeToHole);
    copy_size = Min(from_base->length() - from_start,
                    to_base->length() - to_start);
 if (raw_copy_size == ElementsAccessor::kCopyToEndAndInitializeToHole) {
 int start = to_start + copy_size;
 int length = to_base->length() - start;
 if (length > 0) {
 Heap* heap = from_base->GetHeap();
 MemsetPointer(FixedArray::cast(to_base)->data_start() + start,
                      heap->the_hole_value(), length);
 }
 }
 }
  DCHECK((copy_size + static_cast<int>(to_start)) <= to_base->length() &&
 (copy_size + static_cast<int>(from_start)) <= from_base->length());
 if (copy_size == 0) return;
 FixedArray* from = FixedArray::cast(from_base);
 FixedArray* to = FixedArray::cast(to_base);
  DCHECK(IsFastSmiOrObjectElementsKind(from_kind));
  DCHECK(IsFastSmiOrObjectElementsKind(to_kind));

 WriteBarrierMode write_barrier_mode =
 (IsFastObjectElementsKind(from_kind) && IsFastObjectElementsKind(to_kind))
 ? UPDATE_WRITE_BARRIER
 : SKIP_WRITE_BARRIER;
 for (int i = 0; i < copy_size; i++) {
 Object* value = from->get(from_start + i);
    to->set(to_start + i, value, write_barrier_mode);
 }
}
