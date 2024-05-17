PassRefPtrWillBeRawPtr<RadioNodeList> ContainerNode::radioNodeList(const AtomicString& name, bool onlyMatchImgElements)
{
    ASSERT(isHTMLFormElement(this) || isHTMLFieldSetElement(this));
    CollectionType type = onlyMatchImgElements ? RadioImgNodeListType : RadioNodeListType;
    return ensureCachedCollection<RadioNodeList>(type, name);
}
