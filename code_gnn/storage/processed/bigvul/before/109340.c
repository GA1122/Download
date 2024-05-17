    InspectorThreadableLoaderClient(PassRefPtr<LoadResourceForFrontendCallback> callback)
        : m_callback(callback)
        , m_statusCode(0) { }
