    MainThreadTaskRunner(WTF::MainThreadFunction* function, void* context)
        : m_function(function)
        , m_context(context) { }
