 static void BasicGrowCapacityAndConvertImpl(
 Handle<JSObject> object, Handle<FixedArrayBase> old_elements,
 ElementsKind from_kind, ElementsKind to_kind, uint32_t capacity) {
 Handle<FixedArrayBase> elements =
 ConvertElementsWithCapacity(object, old_elements, from_kind, capacity);

 if (IsHoleyElementsKind(from_kind)) to_kind = GetHoleyElementsKind(to_kind);
 Handle<Map> new_map = JSObject::GetElementsTransitionMap(object, to_kind);
 JSObject::SetMapAndElements(object, new_map, elements);

 JSObject::UpdateAllocationSite(object, to_kind);

 if (FLAG_trace_elements_transitions) {
 JSObject::PrintElementsTransition(stdout, object, from_kind, old_elements,
                                        to_kind, elements);
 }
 }
