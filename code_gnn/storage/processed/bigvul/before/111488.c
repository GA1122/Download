bool InputHandler::openSelectPopup(HTMLSelectElement* select)
{
    if (!select || select->disabled())
        return false;

    if (!select->document()->view())
        return false;

    if (isActiveTextEdit())
        clearCurrentFocusElement();

    m_currentFocusElement = select;
    m_currentFocusElementType = SelectPopup;

    const WTF::Vector<HTMLElement*>& listItems = select->listItems();
    int size = listItems.size();

    bool multiple = select->multiple();
    ScopeArray<BlackBerry::Platform::String> labels;
    labels.reset(new BlackBerry::Platform::String[size]);

    m_webPage->m_page->chrome()->client()->closePagePopup(0);

    bool* enableds = 0;
    int* itemTypes = 0;
    bool* selecteds = 0;

    if (size) {
        enableds = new bool[size];
        itemTypes = new int[size];
        selecteds = new bool[size];
        for (int i = 0; i < size; i++) {
            if (listItems[i]->hasTagName(HTMLNames::optionTag)) {
                HTMLOptionElement* option = static_cast<HTMLOptionElement*>(listItems[i]);
                labels[i] = option->textIndentedToRespectGroupLabel();
                enableds[i] = option->disabled() ? 0 : 1;
                selecteds[i] = option->selected();
                itemTypes[i] = option->parentNode() && option->parentNode()->hasTagName(HTMLNames::optgroupTag) ? TypeOptionInGroup : TypeOption;
            } else if (listItems[i]->hasTagName(HTMLNames::optgroupTag)) {
                HTMLOptGroupElement* optGroup = static_cast<HTMLOptGroupElement*>(listItems[i]);
                labels[i] = optGroup->groupLabelText();
                enableds[i] = optGroup->disabled() ? 0 : 1;
                selecteds[i] = false;
                itemTypes[i] = TypeGroup;
            } else if (listItems[i]->hasTagName(HTMLNames::hrTag)) {
                enableds[i] = false;
                selecteds[i] = false;
                itemTypes[i] = TypeSeparator;
            }
        }
    }

    SelectPopupClient* selectClient = new SelectPopupClient(multiple, size, labels, enableds, itemTypes, selecteds, m_webPage, select);
    WebCore::IntRect elementRectInRootView = select->document()->view()->contentsToRootView(enclosingIntRect(select->getRect()));
    if (!m_webPage->m_page->chrome()->client()->openPagePopup(selectClient, elementRectInRootView))
        m_webPage->m_client->openPopupList(multiple, size, labels, enableds, itemTypes, selecteds);
    delete[] enableds;
    delete[] itemTypes;
    delete[] selecteds;
    return true;
}
