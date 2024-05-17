V8Console::CommandLineAPIScope::~CommandLineAPIScope()
{
    m_cleanup = true;
    v8::Local<v8::Array> names = m_installedMethods->AsArray();
    for (size_t i = 0; i < names->Length(); ++i) {
        v8::Local<v8::Value> name;
        if (!names->Get(m_context, i).ToLocal(&name) || !name->IsName())
            continue;
        if (name->IsString()) {
            v8::Local<v8::Value> descriptor;
            bool success = m_global->GetOwnPropertyDescriptor(m_context, v8::Local<v8::String>::Cast(name)).ToLocal(&descriptor);
            DCHECK(success);
        }
    }
}
