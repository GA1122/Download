Element* Document::getElementById(const AtomicString& id) const
{
    return TreeScope::getElementById(id);
}
