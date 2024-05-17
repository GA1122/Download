void WebPage::clearAutofillData()
{
    if (d->m_webSettings->isFormAutofillEnabled())
        AutofillManager::clear();
}
