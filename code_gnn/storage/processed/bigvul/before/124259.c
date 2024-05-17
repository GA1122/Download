void ScreenOrientation::unlockOrientation(Screen& screen)
{
    ScreenOrientation::from(screen).lockOrientationAsync(WebScreenOrientationDefault);
}
