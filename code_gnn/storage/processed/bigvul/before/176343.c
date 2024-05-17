 static Handle<Object> RemoveElement(Handle<JSArray> receiver,
 Where remove_position) {
 Isolate* isolate = receiver->GetIsolate();
 ElementsKind kind = KindTraits::Kind;
 if (IsFastSmiOrObjectElementsKind(kind)) {
 HandleScope scope(isolate);
 JSObject::EnsureWritableFastElements(receiver);
 }
 Handle<FixedArrayBase> backing_store(receiver->elements(), isolate);
 uint32_t length =
 static_cast<uint32_t>(Smi::cast(receiver->length())->value());
    DCHECK(length > 0);
 int new_length = length - 1;
 int remove_index = remove_position == AT_START ? 0 : new_length;
 Handle<Object> result =
 Subclass::GetImpl(isolate, *backing_store, remove_index);
 if (remove_position == AT_START) {
 Subclass::MoveElements(isolate, receiver, backing_store, 0, 1, new_length,
 0, 0);
 }
 Subclass::SetLengthImpl(isolate, receiver, new_length, backing_store);

 if (IsHoleyElementsKind(kind) && result->IsTheHole(isolate)) {
 return isolate->factory()->undefined_value();
 }
 return result;
 }
