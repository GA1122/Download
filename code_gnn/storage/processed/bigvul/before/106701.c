void WebView::doneWithKeyEvent(const NativeWebKeyboardEvent& event, bool wasEventHandled)
{
    if (!wasEventHandled)
        ::DefWindowProcW(event.nativeEvent()->hwnd, event.nativeEvent()->message, event.nativeEvent()->wParam, event.nativeEvent()->lParam);
}
