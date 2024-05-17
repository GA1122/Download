static bool shouldOpenInNewWindow(Frame* targetFrame, const FrameLoadRequest& request, NavigationPolicy policy)
{
    if (!targetFrame && !request.frameName().isEmpty())
        return true;
    return request.form() && policy != NavigationPolicyCurrentTab;
}
