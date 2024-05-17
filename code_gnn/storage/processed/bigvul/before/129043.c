    void writeRegExp(v8::Handle<v8::Value> value)
    {
        v8::Handle<v8::RegExp> regExp = value.As<v8::RegExp>();
        m_writer.writeRegExp(regExp->GetSource(), regExp->GetFlags());
    }
