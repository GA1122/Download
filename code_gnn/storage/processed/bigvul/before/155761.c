static void acculumateArrayBuffersForAllWorlds(v8::Isolate* isolate, DOMArrayBuffer* object, Vector<v8::Local<v8::ArrayBuffer>, 4>& buffers)
{
    if (isMainThread()) {
        Vector<RefPtr<DOMWrapperWorld>> worlds;
        DOMWrapperWorld::allWorldsInMainThread(worlds);
        for (size_t i = 0; i < worlds.size(); i++) {
            v8::Local<v8::Object> wrapper = worlds[i]->domDataStore().get(object, isolate);
            if (!wrapper.IsEmpty())
                buffers.append(v8::Local<v8::ArrayBuffer>::Cast(wrapper));
        }
    } else {
        v8::Local<v8::Object> wrapper = DOMWrapperWorld::current(isolate).domDataStore().get(object, isolate);
        if (!wrapper.IsEmpty())
            buffers.append(v8::Local<v8::ArrayBuffer>::Cast(wrapper));
    }
}
