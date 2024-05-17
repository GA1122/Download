void InputHandler::sendLearnTextDetails(const WTF::String& string)
{
    Vector<wchar_t> wcharString;
    if (!convertStringToWcharVector(string, wcharString) || wcharString.isEmpty())
        return;

    m_webPage->m_client->inputLearnText(wcharString.data(), wcharString.size());
}
