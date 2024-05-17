 static uint32_t AddArguments(Handle<JSArray> receiver,
 Handle<FixedArrayBase> backing_store,
 Arguments* args, uint32_t add_size,
 Where add_position) {
 uint32_t length = Smi::cast(receiver->length())->value();
    DCHECK(0 < add_size);
 uint32_t elms_len = backing_store->length();
    DCHECK(add_size <= static_cast<uint32_t>(Smi::kMaxValue - length));
 uint32_t new_length = length + add_size;

 if (new_length > elms_len) {
 uint32_t capacity = JSObject::NewElementsCapacity(new_length);
 int copy_dst_index = add_position == AT_START ? add_size : 0;
      backing_store = Subclass::ConvertElementsWithCapacity(
          receiver, backing_store, KindTraits::Kind, capacity, 0,
          copy_dst_index, ElementsAccessor::kCopyToEndAndInitializeToHole);
      receiver->set_elements(*backing_store);
 } else if (add_position == AT_START) {
 Isolate* isolate = receiver->GetIsolate();
 Subclass::MoveElements(isolate, receiver, backing_store, add_size, 0,
                             length, 0, 0);
 }

 int insertion_index = add_position == AT_START ? 0 : length;
 Subclass::CopyArguments(args, backing_store, add_size, 1, insertion_index);
    receiver->set_length(Smi::FromInt(new_length));
 return new_length;
 }
