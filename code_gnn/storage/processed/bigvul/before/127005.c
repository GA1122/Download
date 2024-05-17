bool ChromeClientImpl::canTakeFocus(FocusDirection)
{
    return !layoutTestMode();
}
