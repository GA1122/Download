bool ChromeClientImpl::isPasswordGenerationEnabled() const
{
    return m_webView->passwordGeneratorClient();
}
