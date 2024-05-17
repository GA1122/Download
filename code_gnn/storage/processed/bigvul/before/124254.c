const AtomicString& ScreenOrientation::orientation(Screen& screen)
{
    ScreenOrientation& screenOrientation = ScreenOrientation::from(screen);
    if (!screenOrientation.document()) {
        return orientationToString(blink::WebScreenOrientationPortraitPrimary);
    }
    ScreenOrientationController& controller = ScreenOrientationController::from(*screenOrientation.document());
    return orientationToString(controller.orientation());
}
