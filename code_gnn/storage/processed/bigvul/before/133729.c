bool threadSafeMatch(const Vector<UChar, inlineCapacity>& vector, const QualifiedName& qname)
{
    return equalIgnoringNullity(vector, qname.localName().impl());
}
