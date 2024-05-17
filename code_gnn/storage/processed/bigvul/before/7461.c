void DBusHelperProxy::remoteSignalReceived(int t, const QString &action, QByteArray blob)
{
    SignalType type = static_cast<SignalType>(t);
    QDataStream stream(&blob, QIODevice::ReadOnly);

    if (type == ActionStarted) {
        emit actionStarted(action);
    } else if (type == ActionPerformed) {
        ActionReply reply = ActionReply::deserialize(blob);

        m_actionsInProgress.removeOne(action);
        emit actionPerformed(action, reply);
    } else if (type == DebugMessage) {
        int level;
        QString message;

        stream >> level >> message;

        debugMessageReceived(level, message);
    } else if (type == ProgressStepIndicator) {
        int step;
        stream >> step;

        emit progressStep(action, step);
    } else if (type == ProgressStepData) {
        QVariantMap data;
        stream >> data;

        emit progressStep(action, data);
    }
}
