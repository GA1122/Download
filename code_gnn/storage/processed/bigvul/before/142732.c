void V8HTMLEmbedElement::legacyCallCustom(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    invokeOnScriptableObject<V8HTMLEmbedElement>(info);
    UseCounter::countIfNotPrivateScript(info.GetIsolate(), V8HTMLEmbedElement::toImpl(info.Holder())->document(), UseCounter::HTMLEmbedElementLegacyCall);
}
