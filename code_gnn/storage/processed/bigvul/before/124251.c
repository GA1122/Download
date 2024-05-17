ScreenOrientation& ScreenOrientation::from(Screen& screen)
{
    ScreenOrientation* supplement = static_cast<ScreenOrientation*>(WillBeHeapSupplement<Screen>::from(screen, supplementName()));
    if (!supplement) {
        supplement = new ScreenOrientation(screen);
        provideTo(screen, supplementName(), adoptPtrWillBeNoop(supplement));
    }
    return *supplement;
}
