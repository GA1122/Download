    virtual const char* GetName(v8::Handle<v8::Object> object)
    {
        if (V8DOMWrapper::isWrapperOfType(object, &V8Window::info)) {
            DOMWindow* window = V8Window::toNative(object);
            if (window) {
                CString url = window->document()->url().string().utf8();
                m_strings.append(url);
                return url.data();
            }
        }
        return 0;
    }
