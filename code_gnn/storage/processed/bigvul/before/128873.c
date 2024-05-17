HashSet<SVGElement*>* SVGDocumentExtensions::setOfElementsReferencingTarget(SVGElement* referencedElement) const
{
    ASSERT(referencedElement);
    const HashMap<SVGElement*, OwnPtr<HashSet<SVGElement*> > >::const_iterator it = m_elementDependencies.find(referencedElement);
    if (it == m_elementDependencies.end())
        return 0;
    return it->value.get();
}
