void ChromeClientImpl::openPasswordGenerator(HTMLInputElement* input)
{
    ASSERT(isPasswordGenerationEnabled());
    WebInputElement webInput(input);
    m_webView->passwordGeneratorClient()->openPasswordGenerator(webInput);
}
