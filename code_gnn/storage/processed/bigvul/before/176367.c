 static Handle<FixedArrayBase> SpliceGrowStep(
 Isolate* isolate, Handle<JSArray> receiver,
 Handle<FixedArrayBase> backing_store, uint32_t start,
 uint32_t delete_count, uint32_t add_count, uint32_t length,
 uint32_t new_length) {
    DCHECK((add_count - delete_count) <= (Smi::kMaxValue - length));
 if (new_length <= static_cast<uint32_t>(backing_store->length())) {
 Subclass::MoveElements(isolate, receiver, backing_store,
                             start + add_count, start + delete_count,
 (length - delete_count - start), 0, 0);
 return backing_store;
 }
 int capacity = JSObject::NewElementsCapacity(new_length);
 Handle<FixedArrayBase> new_elms = Subclass::ConvertElementsWithCapacity(
        receiver, backing_store, KindTraits::Kind, capacity, start);
 Subclass::CopyElementsImpl(*backing_store, start + delete_count, *new_elms,
 KindTraits::Kind, start + add_count,
                               kPackedSizeNotKnown,
 ElementsAccessor::kCopyToEndAndInitializeToHole);
    receiver->set_elements(*new_elms);
 return new_elms;
 }
