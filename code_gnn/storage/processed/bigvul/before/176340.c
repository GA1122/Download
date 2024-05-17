 static void ReconfigureImpl(Handle<JSObject> object,
 Handle<FixedArrayBase> store, uint32_t entry,
 Handle<Object> value,
 PropertyAttributes attributes) {
 Handle<FixedArray> parameter_map = Handle<FixedArray>::cast(store);
 uint32_t length = parameter_map->length() - 2;
 Isolate* isolate = store->GetIsolate();
 if (entry < length) {
 Object* probe = parameter_map->get(entry + 2);
      DCHECK(!probe->IsTheHole(isolate));
 Context* context = Context::cast(parameter_map->get(0));
 int context_entry = Smi::cast(probe)->value();
      DCHECK(!context->get(context_entry)->IsTheHole(isolate));
      context->set(context_entry, *value);

      parameter_map->set_the_hole(isolate, entry + 2);
 if ((attributes & READ_ONLY) == 0) {
        value = isolate->factory()->NewAliasedArgumentsEntry(context_entry);
 }

 PropertyDetails details(kData, attributes, 0, PropertyCellType::kNoCell);
 Handle<SeededNumberDictionary> arguments(
 SeededNumberDictionary::cast(parameter_map->get(1)), isolate);
      arguments = SeededNumberDictionary::AddNumberEntry(
          arguments, entry, value, details, object);
      DCHECK_NE(NONE, attributes);
      object->RequireSlowElements(*arguments);
      parameter_map->set(1, *arguments);
 } else {
 Handle<FixedArrayBase> arguments(
 FixedArrayBase::cast(parameter_map->get(1)), isolate);
 DictionaryElementsAccessor::ReconfigureImpl(
          object, arguments, entry - length, value, attributes);
 }
 }
