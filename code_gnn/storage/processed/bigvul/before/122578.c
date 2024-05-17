    virtual void run(Page* page)
    {
        if (m_running)
            return;
        m_running = true;

        WebViewImpl* viewImpl = WebViewImpl::fromPage(page);
        WebDevToolsAgentImpl* agent = static_cast<WebDevToolsAgentImpl*>(viewImpl->devToolsAgent());
        agent->flushPendingFrontendMessages();

        Vector<WebViewImpl*> views;

        const HashSet<Page*>& pages = Page::ordinaryPages();
        HashSet<Page*>::const_iterator end = pages.end();
        for (HashSet<Page*>::const_iterator it =  pages.begin(); it != end; ++it) {
            WebViewImpl* view = WebViewImpl::fromPage(*it);
            if (!view)
                continue;
            m_frozenViews.add(view);
            views.append(view);
            view->setIgnoreInputEvents(true);
        }

        WebView::willEnterModalLoop();

        m_messageLoop->run();

        WebView::didExitModalLoop();

        for (Vector<WebViewImpl*>::iterator it = views.begin(); it != views.end(); ++it) {
            if (m_frozenViews.contains(*it)) {
                (*it)->setIgnoreInputEvents(false);
            }
        }

        m_frozenViews.clear();

        m_running = false;
    }
