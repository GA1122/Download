 static void SpliceShrinkStep(Isolate* isolate, Handle<JSArray> receiver,
 Handle<FixedArrayBase> backing_store,
 uint32_t start, uint32_t delete_count,
 uint32_t add_count, uint32_t len,
 uint32_t new_length) {
 const int move_left_count = len - delete_count - start;
 const int move_left_dst_index = start + add_count;
 Subclass::MoveElements(isolate, receiver, backing_store,
                           move_left_dst_index, start + delete_count,
                           move_left_count, new_length, len);
 }
