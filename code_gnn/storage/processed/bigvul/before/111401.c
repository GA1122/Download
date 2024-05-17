static inline Platform::TouchEventMode toPlatformTouchEventMode(TouchEventMode mode)
{
    switch (mode) {
    case ProcessedTouchEvents:
        return Platform::ProcessedTouchEvents;
    case NativeTouchEvents:
        return Platform::NativeTouchEvents;
    case PureTouchEventsWithMouseConversion:
        return Platform::PureTouchEventsWithMouseConversion;
    default:
        ASSERT_NOT_REACHED();
        return Platform::ProcessedTouchEvents;
    }
}
