 static uint32_t PushImpl(Handle<JSArray> receiver,
 Arguments* args, uint32_t push_size) {
 Handle<FixedArrayBase> backing_store(receiver->elements());
 return Subclass::AddArguments(receiver, backing_store, args, push_size,
                                  AT_END);
 }
