WebPagePrivate::~WebPagePrivate()
{
    m_webPage->setVisible(false);
    if (BackingStorePrivate::currentBackingStoreOwner() == m_webPage)
        BackingStorePrivate::setCurrentBackingStoreOwner(0);

    delete m_webSettings;
    m_webSettings = 0;

    delete m_cookieJar;
    m_cookieJar = 0;

    delete m_webkitThreadViewportAccessor;
    m_webkitThreadViewportAccessor = 0;

    delete m_backingStoreClient;
    m_backingStoreClient = 0;
    m_backingStore = 0;

    delete m_page;
    m_page = 0;

    delete m_transformationMatrix;
    m_transformationMatrix = 0;

    delete m_inPageSearchManager;
    m_inPageSearchManager = 0;

    delete m_selectionHandler;
    m_selectionHandler = 0;

    delete m_inputHandler;
    m_inputHandler = 0;

    delete m_touchEventHandler;
    m_touchEventHandler = 0;

#if !defined(PUBLIC_BUILD) || !PUBLIC_BUILD
    delete m_dumpRenderTree;
    m_dumpRenderTree = 0;
#endif

    AuthenticationChallengeManager::instance()->pageDeleted(this);
}
