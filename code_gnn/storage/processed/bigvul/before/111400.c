static inline Platform::CursorEventMode toPlatformCursorEventMode(CursorEventMode mode)
{
    switch (mode) {
    case ProcessedCursorEvents:
        return Platform::ProcessedCursorEvents;
    case NativeCursorEvents:
        return Platform::NativeCursorEvents;
    default:
        ASSERT_NOT_REACHED();
        return Platform::ProcessedCursorEvents;
    }
}
