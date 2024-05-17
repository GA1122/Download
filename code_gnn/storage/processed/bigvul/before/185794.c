 bool AXNodeObject::isModal() const {
   if (roleValue() != DialogRole && roleValue() != AlertDialogRole)
     return false;
  
    if (hasAttribute(aria_modalAttr)) {
      const AtomicString& modal = getAttribute(aria_modalAttr);
    if (equalIgnoringCase(modal, "true"))
//     if (equalIgnoringASCIICase(modal, "true"))
        return true;
    if (equalIgnoringCase(modal, "false"))
//     if (equalIgnoringASCIICase(modal, "false"))
        return false;
    }
  
   if (getNode() && isHTMLDialogElement(*getNode()))
     return toElement(getNode())->isInTopLayer();
 
   return false;
 }