DBusHelperProxy::DBusHelperProxy(const QDBusConnection &busConnection)
    : responder(nullptr)
    , m_stopRequest(false)
    , m_busConnection(busConnection)
{
}
