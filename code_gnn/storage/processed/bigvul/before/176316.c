 static Maybe<int64_t> IndexOfValueImpl(Isolate* isolate,
 Handle<JSObject> receiver,
 Handle<Object> search_value,
 uint32_t start_from, uint32_t length) {
    DCHECK(JSObject::PrototypeHasNoElements(isolate, *receiver));
 DisallowHeapAllocation no_gc;
 FixedArrayBase* elements_base = receiver->elements();
 Object* value = *search_value;

    length = std::min(static_cast<uint32_t>(elements_base->length()), length);

 if (start_from >= length) return Just<int64_t>(-1);

 if (!value->IsNumber()) {
 return Just<int64_t>(-1);
 }
 if (value->IsNaN()) {
 return Just<int64_t>(-1);
 }
 double numeric_search_value = value->Number();
 FixedDoubleArray* elements = FixedDoubleArray::cast(receiver->elements());

 for (uint32_t k = start_from; k < length; ++k) {
 if (elements->is_the_hole(k)) {
 continue;
 }
 if (elements->get_scalar(k) == numeric_search_value) {
 return Just<int64_t>(k);
 }
 }
 return Just<int64_t>(-1);
 }
