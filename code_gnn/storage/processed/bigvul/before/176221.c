static void CopyDoubleToDoubleElements(FixedArrayBase* from_base,
 uint32_t from_start,
 FixedArrayBase* to_base,
 uint32_t to_start, int raw_copy_size) {
 DisallowHeapAllocation no_allocation;
 int copy_size = raw_copy_size;
 if (raw_copy_size < 0) {
    DCHECK(raw_copy_size == ElementsAccessor::kCopyToEnd ||
           raw_copy_size == ElementsAccessor::kCopyToEndAndInitializeToHole);
    copy_size = Min(from_base->length() - from_start,
                    to_base->length() - to_start);
 if (raw_copy_size == ElementsAccessor::kCopyToEndAndInitializeToHole) {
 for (int i = to_start + copy_size; i < to_base->length(); ++i) {
 FixedDoubleArray::cast(to_base)->set_the_hole(i);
 }
 }
 }
  DCHECK((copy_size + static_cast<int>(to_start)) <= to_base->length() &&
 (copy_size + static_cast<int>(from_start)) <= from_base->length());
 if (copy_size == 0) return;
 FixedDoubleArray* from = FixedDoubleArray::cast(from_base);
 FixedDoubleArray* to = FixedDoubleArray::cast(to_base);
 Address to_address = to->address() + FixedDoubleArray::kHeaderSize;
 Address from_address = from->address() + FixedDoubleArray::kHeaderSize;
  to_address += kDoubleSize * to_start;
  from_address += kDoubleSize * from_start;
 int words_per_double = (kDoubleSize / kPointerSize);
 CopyWords(reinterpret_cast<Object**>(to_address),
 reinterpret_cast<Object**>(from_address),
 static_cast<size_t>(words_per_double * copy_size));
}
