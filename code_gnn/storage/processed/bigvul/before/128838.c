void SVGDocumentExtensions::addElementReferencingTarget(SVGElement* referencingElement, SVGElement* referencedElement)
{
    ASSERT(referencingElement);
    ASSERT(referencedElement);

    if (HashSet<SVGElement*>* elements = m_elementDependencies.get(referencedElement)) {
        elements->add(referencingElement);
        return;
    }

    OwnPtr<HashSet<SVGElement*> > elements = adoptPtr(new HashSet<SVGElement*>);
    elements->add(referencingElement);
    m_elementDependencies.set(referencedElement, elements.release());
}
