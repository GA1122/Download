String WebFrame::markerText(JSObjectRef element)
{
    if (!toJS(element)->inherits(&JSElement::s_info))
        return String();

    return markerTextForListItem(static_cast<JSElement*>(toJS(element))->impl());
}
