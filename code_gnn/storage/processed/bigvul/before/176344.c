 static inline void SetImpl(Handle<JSObject> holder, uint32_t entry,
 Object* value) {
 SetImpl(holder->elements(), entry, value);
 }
