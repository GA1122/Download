 static void AddElementsToKeyAccumulatorImpl(Handle<JSObject> receiver,
 KeyAccumulator* accumulator,
 AddKeyConversion convert) {
 Isolate* isolate = accumulator->isolate();
 Handle<FixedArrayBase> elements(receiver->elements(), isolate);
 uint32_t length = Subclass::GetMaxNumberOfEntries(*receiver, *elements);
 for (uint32_t i = 0; i < length; i++) {
 if (IsFastPackedElementsKind(KindTraits::Kind) ||
 HasEntryImpl(isolate, *elements, i)) {
        accumulator->AddKey(Subclass::GetImpl(isolate, *elements, i), convert);
 }
 }
 }
