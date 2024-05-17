PassRefPtrWillBeRawPtr<ClassCollection> ContainerNode::getElementsByClassName(const AtomicString& classNames)
{
    return ensureCachedCollection<ClassCollection>(ClassCollectionType, classNames);
}
