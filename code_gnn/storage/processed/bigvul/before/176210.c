 static void CollectElementIndicesImpl(Handle<JSObject> object,
 Handle<FixedArrayBase> backing_store,
 KeyAccumulator* keys) {
 Isolate* isolate = keys->isolate();
 uint32_t nof_indices = 0;
 Handle<FixedArray> indices = isolate->factory()->NewFixedArray(
 GetCapacityImpl(*object, *backing_store));
 DirectCollectElementIndicesImpl(isolate, object, backing_store,
 GetKeysConversion::kKeepNumbers,
                                    ENUMERABLE_STRINGS, indices, &nof_indices);
 SortIndices(indices, nof_indices);
 for (uint32_t i = 0; i < nof_indices; i++) {
      keys->AddKey(indices->get(i));
 }
 }
