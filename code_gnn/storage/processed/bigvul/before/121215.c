void HTMLInputElement::onSearch()
{
    ASSERT(isSearchField());
    if (m_inputType)
        static_cast<SearchInputType*>(m_inputType.get())->stopSearchEventTimer();
    dispatchEvent(Event::create(eventNames().searchEvent, true, false));
}