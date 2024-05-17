void HTMLSelectElement::setOption(unsigned index, HTMLOptionElement* option, ExceptionCode& ec)
{
    ec = 0;
    if (index > maxSelectItems - 1)
        index = maxSelectItems - 1;
    int diff = index - length();
    RefPtr<HTMLElement> before = 0;
    if (diff > 0) {
        setLength(index, ec);
    } else if (diff < 0) {
        before = toHTMLElement(options()->item(index+1));
        remove(index);
    }
    if (!ec) {
        add(option, before.get(), ec);
        if (diff >= 0 && option->selected())
            optionSelectionStateChanged(option, true);
    }
}
