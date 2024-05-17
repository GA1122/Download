 static Maybe<int64_t> IndexOfValueImpl(Isolate* isolate,
 Handle<JSObject> receiver,
 Handle<Object> value,
 uint32_t start_from, uint32_t length) {
    DCHECK(JSObject::PrototypeHasNoElements(isolate, *receiver));
 DisallowHeapAllocation no_gc;

 if (WasNeutered(*receiver)) return Just<int64_t>(-1);

 BackingStore* elements = BackingStore::cast(receiver->elements());
 if (!value->IsNumber()) return Just<int64_t>(-1);

 double search_value = value->Number();

 if (!std::isfinite(search_value)) {
 if (AccessorClass::kind() < FLOAT32_ELEMENTS ||
 AccessorClass::kind() > FLOAT64_ELEMENTS) {
 return Just<int64_t>(-1);
 }
 } else if (search_value < std::numeric_limits<ctype>::lowest() ||
               search_value > std::numeric_limits<ctype>::max()) {
 return Just<int64_t>(-1);
 }

 if (static_cast<uint32_t>(elements->length()) < length) {
      length = elements->length();
 }

 if (std::isnan(search_value)) {
 return Just<int64_t>(-1);
 }

    ctype typed_search_value = static_cast<ctype>(search_value);
 if (static_cast<double>(typed_search_value) != search_value) {
 return Just<int64_t>(-1);  
 }

 for (uint32_t k = start_from; k < length; ++k) {
      ctype element_k = elements->get_scalar(k);
 if (element_k == typed_search_value) return Just<int64_t>(k);
 }
 return Just<int64_t>(-1);
 }
