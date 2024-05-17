 void CtcpHandler::handleTime(CtcpType ctcptype, const QString &prefix, const QString &target, const QString &param) {
// void CtcpHandler::handleTime(CtcpType ctcptype, const QString &prefix, const QString &target, const QString &param, QString &reply) {
    Q_UNUSED(target)
    if(ctcptype == CtcpQuery) {
    if(_ignoreListManager->ctcpMatch(prefix, network()->networkName(), "TIME"))
      return;
    reply(nickFromMask(prefix), "TIME", QDateTime::currentDateTime().toString());
//     reply = QDateTime::currentDateTime().toString();
      emit displayMsg(Message::Server, BufferInfo::StatusBuffer, "", tr("Received CTCP TIME request by %1").arg(prefix));
  }
  else {
//   } else {
      emit displayMsg(Message::Server, BufferInfo::StatusBuffer, "", tr("Received CTCP TIME answer from %1: %2")
                      .arg(nickFromMask(prefix)).arg(param));
    }
  }