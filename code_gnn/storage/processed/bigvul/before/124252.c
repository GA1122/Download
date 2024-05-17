bool ScreenOrientation::lockOrientation(Screen& screen, const AtomicString& orientationString)
{
    blink::WebScreenOrientations orientations = stringToOrientations(orientationString);
    if (!orientations)
        return false;
    ScreenOrientation::from(screen).lockOrientationAsync(orientations);
    return true;
}
