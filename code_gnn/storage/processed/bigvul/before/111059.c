void WebPage::applyPendingOrientationIfNeeded()
{
    if (d->m_pendingOrientation != -1)
        d->setScreenOrientation(d->m_pendingOrientation);

    d->m_inputHandler->redrawSpellCheckDialogIfRequired(false  );
}
