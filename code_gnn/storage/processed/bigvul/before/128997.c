    StateBase* startArrayState(v8::Handle<v8::Array> array, StateBase* next)
    {
        v8::Handle<v8::Array> propertyNames = array->GetPropertyNames();
        if (StateBase* newState = checkException(next))
            return newState;
        uint32_t length = array->Length();

        if (shouldSerializeDensely(length, propertyNames->Length())) {
            m_writer.writeGenerateFreshDenseArray(length);
            return push(new DenseArrayState(array, propertyNames, next, isolate()));
        }

        m_writer.writeGenerateFreshSparseArray(length);
        return push(new SparseArrayState(array, propertyNames, next, isolate()));
    }
