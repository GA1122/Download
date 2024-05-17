void IRCView::setServer(Server* newServer)
{
    if (m_server == newServer)
        return;

    m_server = newServer;
}
