void WebPagePrivate::notifyPopupAutofillDialog(const Vector<String>& candidates)
{
    vector<BlackBerry::Platform::String> textItems;
    for (size_t i = 0; i < candidates.size(); i++)
        textItems.push_back(candidates[i]);
    m_client->notifyPopupAutofillDialog(textItems);
}
