void ImageBitmapFactories::ImageBitmapLoader::rejectPromise()
{
    v8::Isolate* isolate = m_scriptState->isolate();
    ScriptScope scope(m_scriptState);
    m_resolver->reject(ScriptValue(v8::Null(isolate), isolate));
    m_factory->didFinishLoading(this);
}
