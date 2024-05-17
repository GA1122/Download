void WebPage::setVisible(bool visible)
{
    if (d->m_visible == visible)
        return;

    d->setVisible(visible);
    AuthenticationChallengeManager::instance()->pageVisibilityChanged(d, visible);

    if (!visible) {
        d->suspendBackingStore();

        size_t foundIndex = visibleWebPages()->find(this);
        if (foundIndex != WTF::notFound)
            visibleWebPages()->remove(foundIndex);

        if (BackingStorePrivate::currentBackingStoreOwner() == this && !visibleWebPages()->isEmpty())
            visibleWebPages()->last()->d->resumeBackingStore();

#if USE(ACCELERATED_COMPOSITING)
        d->suspendRootLayerCommit();
#endif

        return;
    }

#if USE(ACCELERATED_COMPOSITING)
    d->resumeRootLayerCommit();
#endif

    if (!visibleWebPages()->isEmpty() && visibleWebPages()->last() != this) {
        size_t foundIndex = visibleWebPages()->find(this);
        if (foundIndex != WTF::notFound)
            visibleWebPages()->remove(foundIndex);
    }
    visibleWebPages()->append(this);

    if (BackingStorePrivate::currentBackingStoreOwner()
        && BackingStorePrivate::currentBackingStoreOwner() != this)
        BackingStorePrivate::currentBackingStoreOwner()->d->suspendBackingStore();

    d->resumeBackingStore();
}
