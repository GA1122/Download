void WebPage::clearCredentials()
{
#if ENABLE(BLACKBERRY_CREDENTIAL_PERSIST)
    if (d->m_webSettings->isCredentialAutofillEnabled())
        credentialManager().clearCredentials();
#endif
}
