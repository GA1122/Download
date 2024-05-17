 static Handle<Object> GetImpl(Isolate* isolate, FixedArrayBase* parameters,
 uint32_t entry) {
 Handle<FixedArray> parameter_map(FixedArray::cast(parameters), isolate);
 uint32_t length = parameter_map->length() - 2;
 if (entry < length) {
 DisallowHeapAllocation no_gc;
 Object* probe = parameter_map->get(entry + 2);
 Context* context = Context::cast(parameter_map->get(0));
 int context_entry = Smi::cast(probe)->value();
      DCHECK(!context->get(context_entry)->IsTheHole(isolate));
 return handle(context->get(context_entry), isolate);
 } else {
 Handle<Object> result = ArgumentsAccessor::GetImpl(
          isolate, FixedArray::cast(parameter_map->get(1)), entry - length);
 if (result->IsAliasedArgumentsEntry()) {
 DisallowHeapAllocation no_gc;
 AliasedArgumentsEntry* alias = AliasedArgumentsEntry::cast(*result);
 Context* context = Context::cast(parameter_map->get(0));
 int context_entry = alias->aliased_context_slot();
        DCHECK(!context->get(context_entry)->IsTheHole(isolate));
 return handle(context->get(context_entry), isolate);
 }
 return result;
 }
 }
