void Document::setEncoding(const WTF::TextEncoding& encoding)
{
    if (m_encoding == encoding)
        return;

    if (m_titleElement
        && !m_titleElement->firstElementChild()
        && m_encoding == Latin1Encoding()
        && m_titleElement->textContent().containsOnlyLatin1()) {

        CString originalBytes = m_titleElement->textContent().latin1();
        OwnPtr<TextCodec> codec = newTextCodec(encoding);
        String correctlyDecodedTitle = codec->decode(originalBytes.data(), originalBytes.length(), true);
        m_titleElement->setTextContent(correctlyDecodedTitle, IGNORE_EXCEPTION);
    }

    m_encoding = encoding;
}
