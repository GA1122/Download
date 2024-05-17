QByteArray CtcpHandler::xdelimQuote(const QByteArray &message) {
  QByteArray quotedMessage = message;
  QHash<QByteArray, QByteArray>::const_iterator quoteIter = ctcpXDelimDequoteHash.constBegin();
  while(quoteIter != ctcpXDelimDequoteHash.constEnd()) {
    quotedMessage.replace(quoteIter.value(), quoteIter.key());
    quoteIter++;
  }
  return quotedMessage;
}
