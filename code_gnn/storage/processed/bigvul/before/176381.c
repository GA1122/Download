 static void ValidateContents(Handle<JSObject> holder, int length) {
#if DEBUG
 Isolate* isolate = holder->GetIsolate();
 Heap* heap = isolate->heap();
 HandleScope scope(isolate);
 Handle<FixedArrayBase> elements(holder->elements(), isolate);
 Map* map = elements->map();
 if (IsFastSmiOrObjectElementsKind(KindTraits::Kind)) {
      DCHECK_NE(map, heap->fixed_double_array_map());
 } else if (IsFastDoubleElementsKind(KindTraits::Kind)) {
      DCHECK_NE(map, heap->fixed_cow_array_map());
 if (map == heap->fixed_array_map()) DCHECK_EQ(0, length);
 } else {
      UNREACHABLE();
 }
 if (length == 0) return;  
#if ENABLE_SLOW_DCHECKS
 DisallowHeapAllocation no_gc;
 Handle<BackingStore> backing_store = Handle<BackingStore>::cast(elements);
 if (IsFastSmiElementsKind(KindTraits::Kind)) {
 for (int i = 0; i < length; i++) {
        DCHECK(BackingStore::get(*backing_store, i, isolate)->IsSmi() ||
 (IsFastHoleyElementsKind(KindTraits::Kind) &&
                backing_store->is_the_hole(isolate, i)));
 }
 } else if (KindTraits::Kind == FAST_ELEMENTS ||
 KindTraits::Kind == FAST_DOUBLE_ELEMENTS) {
 for (int i = 0; i < length; i++) {
        DCHECK(!backing_store->is_the_hole(isolate, i));
 }
 } else {
      DCHECK(IsFastHoleyElementsKind(KindTraits::Kind));
 }
#endif
#endif
 }
