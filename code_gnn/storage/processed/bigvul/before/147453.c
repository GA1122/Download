void V8TestObject::InstallConditionalFeatures(
    v8::Local<v8::Context> context,
    const DOMWrapperWorld& world,
    v8::Local<v8::Object> instance_object,
    v8::Local<v8::Object> prototype_object,
    v8::Local<v8::Function> interface_object,
    v8::Local<v8::FunctionTemplate> interface_template) {
  CHECK(!interface_template.IsEmpty());
  DCHECK((!prototype_object.IsEmpty() && !interface_object.IsEmpty()) ||
         !instance_object.IsEmpty());

  v8::Isolate* isolate = context->GetIsolate();

  if (!prototype_object.IsEmpty()) {
    v8::Local<v8::Name> unscopables_symbol(v8::Symbol::GetUnscopables(isolate));
    v8::Local<v8::Object> unscopables;
    bool has_unscopables;
    if (prototype_object->HasOwnProperty(context, unscopables_symbol)
        .To(&has_unscopables) && has_unscopables) {
      unscopables = prototype_object->Get(context, unscopables_symbol)
                    .ToLocalChecked().As<v8::Object>();
    } else {
      unscopables = v8::Object::New(isolate);
      unscopables->SetPrototype(context, v8::Null(isolate)).ToChecked();
    }
    unscopables->CreateDataProperty(
        context, V8AtomicString(isolate, "unscopableLongAttribute"), v8::True(isolate))
        .FromJust();
    unscopables->CreateDataProperty(
        context, V8AtomicString(isolate, "unscopableOriginTrialEnabledLongAttribute"), v8::True(isolate))
        .FromJust();
    if (RuntimeEnabledFeatures::RuntimeFeatureEnabled()) {
      unscopables->CreateDataProperty(
          context, V8AtomicString(isolate, "unscopableRuntimeEnabledLongAttribute"), v8::True(isolate))
          .FromJust();
    }
    if (RuntimeEnabledFeatures::RuntimeFeatureEnabled()) {
      unscopables->CreateDataProperty(
          context, V8AtomicString(isolate, "unscopableRuntimeEnabledVoidMethod"), v8::True(isolate))
          .FromJust();
    }
    unscopables->CreateDataProperty(
        context, V8AtomicString(isolate, "unscopableVoidMethod"), v8::True(isolate))
        .FromJust();
    prototype_object->CreateDataProperty(
        context, unscopables_symbol, unscopables).FromJust();
  }
}
