 static uint32_t NumberOfElementsImpl(JSObject* receiver,
 FixedArrayBase* backing_store) {
 uint32_t max_index = Subclass::GetMaxIndex(receiver, backing_store);
 if (IsFastPackedElementsKind(Subclass::kind())) return max_index;
 Isolate* isolate = receiver->GetIsolate();
 uint32_t count = 0;
 for (uint32_t i = 0; i < max_index; i++) {
 if (Subclass::HasEntryImpl(isolate, backing_store, i)) count++;
 }
 return count;
 }
