 static Maybe<bool> IncludesValueImpl(Isolate* isolate,
 Handle<JSObject> receiver,
 Handle<Object> value,
 uint32_t start_from, uint32_t length) {
    DCHECK(JSObject::PrototypeHasNoElements(isolate, *receiver));
 DisallowHeapAllocation no_gc;

 if (WasNeutered(*receiver)) {
 return Just(value->IsUndefined(isolate) && length > start_from);
 }

 BackingStore* elements = BackingStore::cast(receiver->elements());
 if (value->IsUndefined(isolate) &&
        length > static_cast<uint32_t>(elements->length())) {
 return Just(true);
 }
 if (!value->IsNumber()) return Just(false);

 double search_value = value->Number();

 if (!std::isfinite(search_value)) {
 if (AccessorClass::kind() < FLOAT32_ELEMENTS ||
 AccessorClass::kind() > FLOAT64_ELEMENTS) {
 return Just(false);
 }
 } else if (search_value < std::numeric_limits<ctype>::lowest() ||
               search_value > std::numeric_limits<ctype>::max()) {
 return Just(false);
 }

 if (static_cast<uint32_t>(elements->length()) < length) {
      length = elements->length();
 }

 if (!std::isnan(search_value)) {
 for (uint32_t k = start_from; k < length; ++k) {
 double element_k = elements->get_scalar(k);
 if (element_k == search_value) return Just(true);
 }
 return Just(false);
 } else {
 for (uint32_t k = start_from; k < length; ++k) {
 double element_k = elements->get_scalar(k);
 if (std::isnan(element_k)) return Just(true);
 }
 return Just(false);
 }
 }
