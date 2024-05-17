void WebPagePrivate::assignFocus(Platform::FocusDirection direction)
{
    ASSERT((int) Platform::FocusDirectionNone == (int) FocusDirectionNone);
    ASSERT((int) Platform::FocusDirectionForward == (int) FocusDirectionForward);
    ASSERT((int) Platform::FocusDirectionBackward == (int) FocusDirectionBackward);

    clearFocusNode();

    switch (direction) {
    case FocusDirectionForward:
    case FocusDirectionBackward:
        m_page->focusController()->setInitialFocus((FocusDirection) direction, 0);
        break;
    case FocusDirectionNone:
        break;
    default:
        ASSERT_NOT_REACHED();
    }
}
