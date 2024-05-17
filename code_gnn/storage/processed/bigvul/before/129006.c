        static v8::Handle<G> unsafeHandleFromRawValue(const G* value)
        {
            const v8::Handle<G>* handle = reinterpret_cast<const v8::Handle<G>*>(&value);
            return *handle;
        }
