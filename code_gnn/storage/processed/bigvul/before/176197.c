 static void AddElementsToKeyAccumulatorImpl(Handle<JSObject> receiver,
 KeyAccumulator* accumulator,
 AddKeyConversion convert) {
 Isolate* isolate = accumulator->isolate();
 Handle<FixedArrayBase> elements(receiver->elements(), isolate);
 uint32_t length = GetCapacityImpl(*receiver, *elements);
 for (uint32_t entry = 0; entry < length; entry++) {
 if (!HasEntryImpl(isolate, *elements, entry)) continue;
 Handle<Object> value = GetImpl(isolate, *elements, entry);
      accumulator->AddKey(value, convert);
 }
 }
