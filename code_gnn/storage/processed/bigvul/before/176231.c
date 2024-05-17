static void CopyPackedSmiToDoubleElements(FixedArrayBase* from_base,
 uint32_t from_start,
 FixedArrayBase* to_base,
 uint32_t to_start, int packed_size,
 int raw_copy_size) {
 DisallowHeapAllocation no_allocation;
 int copy_size = raw_copy_size;
 uint32_t to_end;
 if (raw_copy_size < 0) {
    DCHECK(raw_copy_size == ElementsAccessor::kCopyToEnd ||
           raw_copy_size == ElementsAccessor::kCopyToEndAndInitializeToHole);
    copy_size = packed_size - from_start;
 if (raw_copy_size == ElementsAccessor::kCopyToEndAndInitializeToHole) {
      to_end = to_base->length();
 for (uint32_t i = to_start + copy_size; i < to_end; ++i) {
 FixedDoubleArray::cast(to_base)->set_the_hole(i);
 }
 } else {
      to_end = to_start + static_cast<uint32_t>(copy_size);
 }
 } else {
    to_end = to_start + static_cast<uint32_t>(copy_size);
 }
  DCHECK(static_cast<int>(to_end) <= to_base->length());
  DCHECK(packed_size >= 0 && packed_size <= copy_size);
  DCHECK((copy_size + static_cast<int>(to_start)) <= to_base->length() &&
 (copy_size + static_cast<int>(from_start)) <= from_base->length());
 if (copy_size == 0) return;
 FixedArray* from = FixedArray::cast(from_base);
 FixedDoubleArray* to = FixedDoubleArray::cast(to_base);
 for (uint32_t from_end = from_start + static_cast<uint32_t>(packed_size);
       from_start < from_end; from_start++, to_start++) {
 Object* smi = from->get(from_start);
    DCHECK(!smi->IsTheHole(from->GetIsolate()));
    to->set(to_start, Smi::cast(smi)->value());
 }
}
