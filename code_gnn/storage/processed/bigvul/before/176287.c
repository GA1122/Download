 static uint32_t GetMaxIndex(JSObject* receiver, FixedArrayBase* elements) {
 if (receiver->IsJSArray()) {
      DCHECK(JSArray::cast(receiver)->length()->IsSmi());
 return static_cast<uint32_t>(
 Smi::cast(JSArray::cast(receiver)->length())->value());
 }
 return Subclass::GetCapacityImpl(receiver, elements);
 }
