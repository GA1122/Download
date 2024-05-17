static inline void notifyTargetAndInstancesAboutAnimValChange(SVGElement* targetElement, const QualifiedName& attributeName)
{
    ASSERT(targetElement);
    if (attributeName == anyQName() || !targetElement->inDocument() || !targetElement->parentNode())
        return;

    SVGElementInstance::InstanceUpdateBlocker blocker(targetElement);
    notifyTargetAboutAnimValChange(targetElement, attributeName);

    const HashSet<SVGElementInstance*>& instances = targetElement->instancesForElement();
    const HashSet<SVGElementInstance*>::const_iterator end = instances.end();
    for (HashSet<SVGElementInstance*>::const_iterator it = instances.begin(); it != end; ++it) {
        if (SVGElement* shadowTreeElement = (*it)->shadowTreeElement())
            notifyTargetAboutAnimValChange(shadowTreeElement, attributeName);
    }
}
