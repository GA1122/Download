HTMLDocument::HTMLDocument(const DocumentInit& initializer, DocumentClassFlags extendedDocumentClasses)
    : Document(initializer, HTMLDocumentClass | extendedDocumentClasses)
{
    ScriptWrappable::init(this);
    clearXMLVersion();
}
