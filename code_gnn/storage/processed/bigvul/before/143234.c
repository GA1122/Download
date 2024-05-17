Document::PageDismissalType Document::pageDismissalEventBeingDispatched() const
{
    switch (m_loadEventProgress) {
    case BeforeUnloadEventInProgress:
        return BeforeUnloadDismissal;
    case PageHideInProgress:
        return PageHideDismissal;
    case UnloadVisibilityChangeInProgress:
        return UnloadVisibilityChangeDismissal;
    case UnloadEventInProgress:
        return UnloadDismissal;

    case LoadEventNotRun:
    case LoadEventInProgress:
    case LoadEventCompleted:
    case BeforeUnloadEventCompleted:
    case UnloadEventHandled:
        return NoDismissal;
    }
    ASSERT_NOT_REACHED();
    return NoDismissal;
}
