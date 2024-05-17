static void SortIndices(
 Handle<FixedArray> indices, uint32_t sort_size,
 WriteBarrierMode write_barrier_mode = UPDATE_WRITE_BARRIER) {
 struct {
 bool operator()(Object* a, Object* b) {
 if (a->IsSmi() || !a->IsUndefined(HeapObject::cast(a)->GetIsolate())) {
 if (!b->IsSmi() && b->IsUndefined(HeapObject::cast(b)->GetIsolate())) {
 return true;
 }
 return a->Number() < b->Number();
 }
 return !b->IsSmi() && b->IsUndefined(HeapObject::cast(b)->GetIsolate());
 }
 } cmp;
 Object** start =
 reinterpret_cast<Object**>(indices->GetFirstElementAddress());
  std::sort(start, start + sort_size, cmp);
 if (write_barrier_mode != SKIP_WRITE_BARRIER) {
    FIXED_ARRAY_ELEMENTS_WRITE_BARRIER(indices->GetIsolate()->heap(), *indices,
 0, sort_size);
 }
}
