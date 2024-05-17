void Document::setCharset(const String& charset)
{
    if (!decoder())
        return;
    decoder()->setEncoding(charset, TextResourceDecoder::UserChosenEncoding);
}