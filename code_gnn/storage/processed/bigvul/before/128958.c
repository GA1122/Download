static void neuterArrayBufferInAllWorlds(ArrayBuffer* object)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    if (isMainThread()) {
        Vector<RefPtr<DOMWrapperWorld> > worlds;
        DOMWrapperWorld::allWorldsInMainThread(worlds);
        for (size_t i = 0; i < worlds.size(); i++) {
            v8::Handle<v8::Object> wrapper = worlds[i]->domDataStore().get<V8ArrayBuffer>(object, isolate);
            if (!wrapper.IsEmpty()) {
                ASSERT(wrapper->IsArrayBuffer());
                v8::Handle<v8::ArrayBuffer>::Cast(wrapper)->Neuter();
            }
        }
    } else {
        v8::Handle<v8::Object> wrapper = DOMWrapperWorld::current(isolate).domDataStore().get<V8ArrayBuffer>(object, isolate);
        if (!wrapper.IsEmpty()) {
            ASSERT(wrapper->IsArrayBuffer());
            v8::Handle<v8::ArrayBuffer>::Cast(wrapper)->Neuter();
        }
    }
}
