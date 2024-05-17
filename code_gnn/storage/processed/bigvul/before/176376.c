 static void TryTransitionResultArrayToPacked(Handle<JSArray> array) {
 if (!IsHoleyElementsKind(kind())) return;
 int length = Smi::cast(array->length())->value();
 Handle<FixedArrayBase> backing_store(array->elements());
 if (!Subclass::IsPackedImpl(array, backing_store, 0, length)) {
 return;
 }
 ElementsKind packed_kind = GetPackedElementsKind(kind());
 Handle<Map> new_map =
 JSObject::GetElementsTransitionMap(array, packed_kind);
 JSObject::MigrateToMap(array, new_map);
 if (FLAG_trace_elements_transitions) {
 JSObject::PrintElementsTransition(stdout, array, kind(), backing_store,
                                        packed_kind, backing_store);
 }
 }
