 static void AddElementsToKeyAccumulatorImpl(Handle<JSObject> receiver,
 KeyAccumulator* accumulator,
 AddKeyConversion convert) {
 Isolate* isolate = receiver->GetIsolate();
 Handle<FixedArrayBase> elements(receiver->elements());
 uint32_t length = AccessorClass::GetCapacityImpl(*receiver, *elements);
 for (uint32_t i = 0; i < length; i++) {
 Handle<Object> value = AccessorClass::GetImpl(isolate, *elements, i);
      accumulator->AddKey(value, convert);
 }
 }
