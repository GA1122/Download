bool DBusHelperProxy::initHelper(const QString &name)
{
    new Kf5authAdaptor(this);

    if (!m_busConnection.registerService(name)) {
        qCWarning(KAUTH) << "Error registering helper DBus service" << name << m_busConnection.lastError().message();
        return false;
    }

    if (!m_busConnection.registerObject(QLatin1String("/"), this)) {
        qCWarning(KAUTH) << "Error registering helper DBus object:" << m_busConnection.lastError().message();
        return false;
    }

    m_name = name;

    return true;
}
