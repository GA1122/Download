void V8HTMLEmbedElement::namedPropertySetterCustom(v8::Local<v8::Name> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    setScriptableObjectProperty<V8HTMLEmbedElement>(name.As<v8::String>(), value, info);
}
