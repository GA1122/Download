 static void CopyArguments(Arguments* args, Handle<FixedArrayBase> dst_store,
 uint32_t copy_size, uint32_t src_index,
 uint32_t dst_index) {
 DisallowHeapAllocation no_gc;
 FixedArrayBase* raw_backing_store = *dst_store;
 WriteBarrierMode mode = raw_backing_store->GetWriteBarrierMode(no_gc);
 for (uint32_t i = 0; i < copy_size; i++) {
 Object* argument = (*args)[src_index + i];
      DCHECK(!argument->IsTheHole(raw_backing_store->GetIsolate()));
 Subclass::SetImpl(raw_backing_store, dst_index + i, argument, mode);
 }
 }
