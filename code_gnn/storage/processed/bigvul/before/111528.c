void InputHandler::spellCheckingRequestCancelled(int32_t transactionId)
{
    SpellingLog(LogLevelWarn, "InputHandler::spellCheckingRequestCancelled Expected transaction id %d, received %d. %s"
                , transactionId
                , m_processingTransactionId
                , transactionId == m_processingTransactionId ? "" : "We are out of sync with input service.");

    m_request->didCancel();
    m_processingTransactionId = -1;
}
