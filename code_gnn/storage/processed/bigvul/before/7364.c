CtcpHandler::CtcpHandler(CoreNetwork *parent)
  : CoreBasicHandler(parent),
    XDELIM("\001"),
    _ignoreListManager(parent->ignoreListManager())
{

  QByteArray MQUOTE = QByteArray("\020");
  ctcpMDequoteHash[MQUOTE + '0'] = QByteArray(1, '\000');
  ctcpMDequoteHash[MQUOTE + 'n'] = QByteArray(1, '\n');
  ctcpMDequoteHash[MQUOTE + 'r'] = QByteArray(1, '\r');
  ctcpMDequoteHash[MQUOTE + MQUOTE] = MQUOTE;

  QByteArray XQUOTE = QByteArray("\134");
  ctcpXDelimDequoteHash[XQUOTE + XQUOTE] = XQUOTE;
  ctcpXDelimDequoteHash[XQUOTE + QByteArray("a")] = XDELIM;
}
