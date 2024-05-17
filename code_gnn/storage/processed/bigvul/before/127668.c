void KeyboardClickableInputTypeView::accessKeyAction(bool sendMouseEvents)
{
    InputTypeView::accessKeyAction(sendMouseEvents);
    element().dispatchSimulatedClick(0, sendMouseEvents ? SendMouseUpDownEvents : SendNoEvents);
}
