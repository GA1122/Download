String Document::suggestedMIMEType() const
{
    if (isXHTMLDocument())
        return "application/xhtml+xml";
    if (isSVGDocument())
        return "image/svg+xml";
    if (xmlStandalone())
        return "text/xml";
    if (isHTMLDocument())
        return "text/html";

    if (DocumentLoader* documentLoader = loader())
        return documentLoader->responseMIMEType();
    return String();
}
