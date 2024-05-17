DBusHelperProxy::DBusHelperProxy()
    : responder(nullptr)
    , m_stopRequest(false)
    , m_busConnection(QDBusConnection::systemBus())
{
}
