void SVGDocumentExtensions::removeAllTargetReferencesForElement(SVGElement* referencingElement)
{
    Vector<SVGElement*> toBeRemoved;

    HashMap<SVGElement*, OwnPtr<HashSet<SVGElement*> > >::iterator end = m_elementDependencies.end();
    for (HashMap<SVGElement*, OwnPtr<HashSet<SVGElement*> > >::iterator it = m_elementDependencies.begin(); it != end; ++it) {
        SVGElement* referencedElement = it->key;
        HashSet<SVGElement*>* referencingElements = it->value.get();
        HashSet<SVGElement*>::iterator setIt = referencingElements->find(referencingElement);
        if (setIt == referencingElements->end())
            continue;

        referencingElements->remove(setIt);
        if (referencingElements->isEmpty())
            toBeRemoved.append(referencedElement);
    }

    m_elementDependencies.removeAll(toBeRemoved);
}
