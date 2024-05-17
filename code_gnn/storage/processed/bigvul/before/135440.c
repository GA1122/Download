static Widget* widgetForElement(const Element& focusedElement)
{
    LayoutObject* layoutObject = focusedElement.layoutObject();
    if (!layoutObject || !layoutObject->isLayoutPart())
        return 0;
    return toLayoutPart(layoutObject)->widget();
}
