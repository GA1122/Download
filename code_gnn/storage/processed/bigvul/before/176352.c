 static inline void SetImpl(FixedArrayBase* store, uint32_t entry,
 Object* value) {
 FixedArray* parameter_map = FixedArray::cast(store);
 uint32_t length = parameter_map->length() - 2;
 if (entry < length) {
 Object* probe = parameter_map->get(entry + 2);
 Context* context = Context::cast(parameter_map->get(0));
 int context_entry = Smi::cast(probe)->value();
      DCHECK(!context->get(context_entry)->IsTheHole(store->GetIsolate()));
      context->set(context_entry, value);
 } else {
 FixedArray* arguments = FixedArray::cast(parameter_map->get(1));
 Object* current = ArgumentsAccessor::GetRaw(arguments, entry - length);
 if (current->IsAliasedArgumentsEntry()) {
 AliasedArgumentsEntry* alias = AliasedArgumentsEntry::cast(current);
 Context* context = Context::cast(parameter_map->get(0));
 int context_entry = alias->aliased_context_slot();
        DCHECK(!context->get(context_entry)->IsTheHole(store->GetIsolate()));
        context->set(context_entry, value);
 } else {
 ArgumentsAccessor::SetImpl(arguments, entry - length, value);
 }
 }
 }
