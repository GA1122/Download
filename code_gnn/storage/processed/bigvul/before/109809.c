void Document::setDecoder(PassRefPtr<TextResourceDecoder> decoder)
{
    m_decoder = decoder;
    setEncoding(m_decoder ? m_decoder->encoding() : WTF::TextEncoding());
}
