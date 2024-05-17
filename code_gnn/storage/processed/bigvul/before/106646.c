void WebPageProxy::showContextMenu(const IntPoint& menuLocation, const ContextMenuState& contextMenuState, const Vector<WebContextMenuItemData>& proposedItems, CoreIPC::ArgumentDecoder* arguments)
{
    RefPtr<APIObject> userData;
    WebContextUserMessageDecoder messageDecoder(userData, context());
    if (!arguments->decode(messageDecoder))
        return;

    m_activeContextMenuState = contextMenuState;

    if (m_activeContextMenu) {
        m_activeContextMenu->hideContextMenu();
        m_activeContextMenu = 0;
    }

    m_activeContextMenu = m_pageClient->createContextMenuProxy(this);

    process()->responsivenessTimer()->stop();

    Vector<WebContextMenuItemData> items;
    if (!m_contextMenuClient.getContextMenuFromProposedMenu(this, proposedItems, items, userData.get()))
        m_activeContextMenu->showContextMenu(menuLocation, proposedItems);
    else
        m_activeContextMenu->showContextMenu(menuLocation, items);
}
