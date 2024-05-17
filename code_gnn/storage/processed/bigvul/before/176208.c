 static void CollectElementIndicesImpl(Handle<JSObject> object,
 Handle<FixedArrayBase> backing_store,
 KeyAccumulator* keys) {
    DCHECK_NE(DICTIONARY_ELEMENTS, kind());
 uint32_t length = Subclass::GetMaxIndex(*object, *backing_store);
 PropertyFilter filter = keys->filter();
 Isolate* isolate = keys->isolate();
 Factory* factory = isolate->factory();
 for (uint32_t i = 0; i < length; i++) {
 if (Subclass::HasElementImpl(isolate, object, i, backing_store, filter)) {
        keys->AddKey(factory->NewNumberFromUint(i));
 }
 }
 }
