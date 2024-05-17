static void CopyDoubleToObjectElements(FixedArrayBase* from_base,
 uint32_t from_start,
 FixedArrayBase* to_base,
 uint32_t to_start, int raw_copy_size) {
 int copy_size = raw_copy_size;
 if (raw_copy_size < 0) {
 DisallowHeapAllocation no_allocation;
    DCHECK(raw_copy_size == ElementsAccessor::kCopyToEnd ||
           raw_copy_size == ElementsAccessor::kCopyToEndAndInitializeToHole);
    copy_size = Min(from_base->length() - from_start,
                    to_base->length() - to_start);
 if (raw_copy_size == ElementsAccessor::kCopyToEndAndInitializeToHole) {
 int start = to_start;
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

 Isolate* isolate = from_base->GetIsolate();
 Handle<FixedDoubleArray> from(FixedDoubleArray::cast(from_base), isolate);
 Handle<FixedArray> to(FixedArray::cast(to_base), isolate);

 int offset = 0;
 while (offset < copy_size) {
 HandleScope scope(isolate);
    offset += 100;
 for (int i = offset - 100; i < offset && i < copy_size; ++i) {
 Handle<Object> value =
 FixedDoubleArray::get(*from, i + from_start, isolate);
      to->set(i + to_start, *value, UPDATE_WRITE_BARRIER);
 }
 }
}
