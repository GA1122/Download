void XMLHttpRequest::setRequestHeaderInternal(const AtomicString& name, const String& value)
{
    HTTPHeaderMap::AddResult result = m_requestHeaders.add(name, value);
    if (!result.isNewEntry)
        result.iterator->value = result.iterator->value + ", " + value;
}
