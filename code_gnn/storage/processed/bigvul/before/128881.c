        DenseArrayState(v8::Handle<v8::Array> array, v8::Handle<v8::Array> propertyNames, StateBase* next, v8::Isolate* isolate)
            : AbstractObjectState(array, next)
            , m_arrayIndex(0)
            , m_arrayLength(array->Length())
        {
            m_propertyNames = v8::Local<v8::Array>::New(isolate, propertyNames);
        }
