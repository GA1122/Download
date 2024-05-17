 static Maybe<bool> IncludesValueImpl(Isolate* isolate,
 Handle<JSObject> receiver,
 Handle<Object> search_value,
 uint32_t start_from, uint32_t length) {
    DCHECK(JSObject::PrototypeHasNoElements(isolate, *receiver));
 DisallowHeapAllocation no_gc;
 FixedArrayBase* elements_base = receiver->elements();
 Object* the_hole = isolate->heap()->the_hole_value();
 Object* undefined = isolate->heap()->undefined_value();
 Object* value = *search_value;

 if (value == undefined &&
 static_cast<uint32_t>(elements_base->length()) < length) {
 return Just(true);
 }

 if (start_from >= length) return Just(false);

    length = std::min(static_cast<uint32_t>(elements_base->length()), length);

 if (!value->IsNumber()) {
 if (value == undefined) {
 if (!IsFastObjectElementsKind(Subclass::kind()) &&
 !IsFastHoleyElementsKind(Subclass::kind())) {
 return Just(false);
 }

 if (IsFastSmiOrObjectElementsKind(Subclass::kind())) {
 auto elements = FixedArray::cast(receiver->elements());

 for (uint32_t k = start_from; k < length; ++k) {
 Object* element_k = elements->get(k);

 if (IsFastHoleyElementsKind(Subclass::kind()) &&
                element_k == the_hole) {
 return Just(true);
 }
 if (IsFastObjectElementsKind(Subclass::kind()) &&
                element_k == undefined) {
 return Just(true);
 }
 }
 return Just(false);
 } else {
          DCHECK_EQ(Subclass::kind(), FAST_HOLEY_DOUBLE_ELEMENTS);
 auto elements = FixedDoubleArray::cast(receiver->elements());

 for (uint32_t k = start_from; k < length; ++k) {
 if (IsFastHoleyElementsKind(Subclass::kind()) &&
                elements->is_the_hole(k)) {
 return Just(true);
 }
 }
 return Just(false);
 }
 } else if (!IsFastObjectElementsKind(Subclass::kind())) {
 return Just(false);
 } else {
        DCHECK(IsFastObjectElementsKind(Subclass::kind()));
 auto elements = FixedArray::cast(receiver->elements());

 for (uint32_t k = start_from; k < length; ++k) {
 Object* element_k = elements->get(k);
 if (IsFastHoleyElementsKind(Subclass::kind()) &&
              element_k == the_hole) {
 continue;
 }

 if (value->SameValueZero(element_k)) return Just(true);
 }
 return Just(false);
 }
 } else {
 if (!value->IsNaN()) {
 double search_value = value->Number();
 if (IsFastDoubleElementsKind(Subclass::kind())) {
 auto elements = FixedDoubleArray::cast(receiver->elements());

 for (uint32_t k = start_from; k < length; ++k) {
 if (IsFastHoleyElementsKind(Subclass::kind()) &&
                elements->is_the_hole(k)) {
 continue;
 }
 if (elements->get_scalar(k) == search_value) return Just(true);
 }
 return Just(false);
 } else {
 auto elements = FixedArray::cast(receiver->elements());

 for (uint32_t k = start_from; k < length; ++k) {
 Object* element_k = elements->get(k);
 if (element_k->IsNumber() && element_k->Number() == search_value) {
 return Just(true);
 }
 }
 return Just(false);
 }
 } else {
 if (IsFastSmiElementsKind(Subclass::kind())) return Just(false);

 if (IsFastDoubleElementsKind(Subclass::kind())) {
 auto elements = FixedDoubleArray::cast(receiver->elements());

 for (uint32_t k = start_from; k < length; ++k) {
 if (IsFastHoleyElementsKind(Subclass::kind()) &&
                elements->is_the_hole(k)) {
 continue;
 }
 if (std::isnan(elements->get_scalar(k))) return Just(true);
 }
 return Just(false);
 } else {
          DCHECK(IsFastSmiOrObjectElementsKind(Subclass::kind()));
 auto elements = FixedArray::cast(receiver->elements());

 for (uint32_t k = start_from; k < length; ++k) {
 if (elements->get(k)->IsNaN()) return Just(true);
 }
 return Just(false);
 }
 }
 }
 }
