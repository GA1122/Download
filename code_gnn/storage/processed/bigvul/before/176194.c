 static void AddElementsToKeyAccumulatorImpl(Handle<JSObject> receiver,
 KeyAccumulator* accumulator,
 AddKeyConversion convert) {
 Isolate* isolate = accumulator->isolate();
 Handle<Object> undefined = isolate->factory()->undefined_value();
 Handle<Object> the_hole = isolate->factory()->the_hole_value();
 Handle<SeededNumberDictionary> dictionary(
 SeededNumberDictionary::cast(receiver->elements()), isolate);
 int capacity = dictionary->Capacity();
 for (int i = 0; i < capacity; i++) {
 Object* k = dictionary->KeyAt(i);
 if (k == *undefined) continue;
 if (k == *the_hole) continue;
 if (dictionary->IsDeleted(i)) continue;
 Object* value = dictionary->ValueAt(i);
      DCHECK(!value->IsTheHole(isolate));
      DCHECK(!value->IsAccessorPair());
      DCHECK(!value->IsAccessorInfo());
      accumulator->AddKey(value, convert);
 }
 }
