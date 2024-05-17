    v8::Local<v8::Object> ensureConsole()
    {
        if (m_console.IsEmpty()) {
            DCHECK(!m_info.Data().IsEmpty());
            DCHECK(!m_info.Data()->IsUndefined());
            m_console = m_info.Data().As<v8::Object>();
        }
        return m_console;
    }
