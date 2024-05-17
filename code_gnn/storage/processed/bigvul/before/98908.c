bool causesFosterParenting(const QualifiedName& tagName)
{
    return tagName == tableTag
        || tagName == tbodyTag
        || tagName == tfootTag
        || tagName == theadTag
        || tagName == trTag;
}
