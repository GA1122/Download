 static void DeleteCommon(Handle<JSObject> obj, uint32_t entry,
 Handle<FixedArrayBase> store) {
    DCHECK(obj->HasFastSmiOrObjectElements() || obj->HasFastDoubleElements() ||
           obj->HasFastArgumentsElements() ||
           obj->HasFastStringWrapperElements());
 Handle<BackingStore> backing_store = Handle<BackingStore>::cast(store);
 if (!obj->IsJSArray() &&
        entry == static_cast<uint32_t>(store->length()) - 1) {
 DeleteAtEnd(obj, backing_store, entry);
 return;
 }

 Isolate* isolate = obj->GetIsolate();
    backing_store->set_the_hole(isolate, entry);

 const int kMinLengthForSparsenessCheck = 64;
 if (backing_store->length() < kMinLengthForSparsenessCheck) return;
 if (backing_store->GetHeap()->InNewSpace(*backing_store)) return;
 uint32_t length = 0;
 if (obj->IsJSArray()) {
 JSArray::cast(*obj)->length()->ToArrayLength(&length);
 } else {
      length = static_cast<uint32_t>(store->length());
 }
 if ((entry > 0 && backing_store->is_the_hole(isolate, entry - 1)) ||
 (entry + 1 < length &&
         backing_store->is_the_hole(isolate, entry + 1))) {
 if (!obj->IsJSArray()) {
 uint32_t i;
 for (i = entry + 1; i < length; i++) {
 if (!backing_store->is_the_hole(isolate, i)) break;
 }
 if (i == length) {
 DeleteAtEnd(obj, backing_store, entry);
 return;
 }
 }
 int num_used = 0;
 for (int i = 0; i < backing_store->length(); ++i) {
 if (!backing_store->is_the_hole(isolate, i)) {
 ++num_used;
 if (4 * SeededNumberDictionary::ComputeCapacity(num_used) *
 SeededNumberDictionary::kEntrySize >
              backing_store->length()) {
 return;
 }
 }
 }
 JSObject::NormalizeElements(obj);
 }
 }
