void HTMLSelectElement::setLength(unsigned newLen, ExceptionCode& ec)
{
    ec = 0;
    if (newLen > maxSelectItems)
        newLen = maxSelectItems;
    int diff = length() - newLen;

    if (diff < 0) {  
        do {
            RefPtr<Element> option = document()->createElement(optionTag, false);
            ASSERT(option);
            add(toHTMLElement(option.get()), 0, ec);
            if (ec)
                break;
        } while (++diff);
    } else {
        const Vector<HTMLElement*>& items = listItems();

        Vector<RefPtr<Element> > itemsToRemove;
        size_t optionIndex = 0;
        for (size_t i = 0; i < items.size(); ++i) {
            Element* item = items[i];
            if (item->hasLocalName(optionTag) && optionIndex++ >= newLen) {
                ASSERT(item->parentNode());
                itemsToRemove.append(item);
            }
        }

        for (size_t i = 0; i < itemsToRemove.size(); ++i) {
            Element* item = itemsToRemove[i].get();
            if (item->parentNode())
                item->parentNode()->removeChild(item, ec);
        }
    }
    setNeedsValidityCheck();
}
