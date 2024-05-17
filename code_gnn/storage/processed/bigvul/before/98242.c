String WebFrame::contentsAsString() const 
{
    if (!m_coreFrame)
        return String();

    if (isFrameSet()) {
        StringBuilder builder;
        for (Frame* child = m_coreFrame->tree()->firstChild(); child; child = child->tree()->nextSibling()) {
            if (!builder.isEmpty())
                builder.append(' ');
            builder.append(static_cast<WebFrameLoaderClient*>(child->loader()->client())->webFrame()->contentsAsString());
        }
        return builder.toString();
    }

    Document* document = m_coreFrame->document();
    if (!document)
        return String();

    RefPtr<Element> documentElement = document->documentElement();
    if (!documentElement)
        return String();

    RefPtr<Range> range = document->createRange();

    ExceptionCode ec = 0;
    range->selectNode(documentElement.get(), ec);
    if (ec)
        return String();

    return plainText(range.get());
}
