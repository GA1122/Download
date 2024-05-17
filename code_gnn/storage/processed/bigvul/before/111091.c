void WebPage::clearNeverRememberSites()
{
#if ENABLE(BLACKBERRY_CREDENTIAL_PERSIST)
    if (d->m_webSettings->isCredentialAutofillEnabled())
        credentialManager().clearNeverRememberSites();
#endif
}
