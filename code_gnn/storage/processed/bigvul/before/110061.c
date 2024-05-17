void HTMLSelectElement::setMultiple(bool multiple)
{
    bool oldMultiple = this->multiple();
    int oldSelectedIndex = selectedIndex();
    setAttribute(multipleAttr, multiple ? "" : 0);

    if (oldMultiple != this->multiple())
        setSelectedIndex(oldSelectedIndex);
}
