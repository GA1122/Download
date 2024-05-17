bool Polkit1Backend::actionExists(const QString &action)
{
    if (m_flyingActions) {
        int tries = 0;
        while (m_flyingActions && tries < 10) {
            QEventLoop e;
            QTimer::singleShot(200, &e, SLOT(quit()));
            e.exec();
            ++tries;
        }
    }

    return m_knownActions.contains(action);
}
