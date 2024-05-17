 static uint32_t UnshiftImpl(Handle<JSArray> receiver,
 Arguments* args, uint32_t unshift_size) {
 Handle<FixedArrayBase> backing_store(receiver->elements());
 return Subclass::AddArguments(receiver, backing_store, args, unshift_size,
                                  AT_START);
 }
