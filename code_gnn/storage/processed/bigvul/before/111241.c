void WebPage::notifyAppActivationStateChange(ActivationStateType activationState)
{
#if ENABLE(VIDEO)
    MediaPlayerPrivate::notifyAppActivatedEvent(activationState == ActivationActive);
#endif

    FOR_EACH_PLUGINVIEW(d->m_pluginViews) {
        switch (activationState) {
        case ActivationActive:
            (*it)->handleAppActivatedEvent();
            break;
        case ActivationInactive:
            (*it)->handleAppDeactivatedEvent();
            break;
        case ActivationStandby:
            (*it)->handleAppStandbyEvent();
            break;
        }
    }

    d->notifyAppActivationStateChange(activationState);
}
