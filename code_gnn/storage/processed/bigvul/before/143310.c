bool Frame::canNavigate(const Frame& targetFrame)
{
    String errorReason;
    bool isAllowedNavigation = canNavigateWithoutFramebusting(targetFrame, errorReason);

    if (targetFrame != this && !securityContext()->isSandboxed(SandboxTopNavigation) && targetFrame == tree().top()) {
        DEFINE_STATIC_LOCAL(EnumerationHistogram, framebustHistogram, ("WebCore.Framebust", 4));
        const unsigned userGestureBit = 0x1;
        const unsigned allowedBit = 0x2;
        unsigned framebustParams = 0;
        if (UserGestureIndicator::processingUserGesture())
            framebustParams |= userGestureBit;
        if (isAllowedNavigation)
            framebustParams |= allowedBit;
        framebustHistogram.count(framebustParams);
        return true;
    }
    if (!isAllowedNavigation && !errorReason.isNull())
        printNavigationErrorMessage(targetFrame, errorReason.latin1().data());
    return isAllowedNavigation;
}
