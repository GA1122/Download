void DBusHelperProxy::stopAction(const QString &action)
{
    Q_UNUSED(action)
    m_stopRequest = true;
}
