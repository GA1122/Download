static void addLocalNameToSet(HashSet<StringImpl*>* set, const QualifiedName& qName)
{
    set->add(qName.localName().impl());
}
