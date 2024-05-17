void WebLocalFrameImpl::AdvanceFocusInForm(WebFocusType focus_type) {
  DCHECK(GetFrame()->GetDocument());
  Element* element = GetFrame()->GetDocument()->FocusedElement();
  if (!element)
    return;

  Element* next_element =
      GetFrame()->GetPage()->GetFocusController().NextFocusableElementInForm(
          element, focus_type);
  if (!next_element)
    return;

  next_element->scrollIntoViewIfNeeded(true  );
  next_element->focus();
}
