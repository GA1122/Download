bool SerializedScriptValue::containsTransferableArrayBuffer() const
{
    return m_arrayBufferContentsArray && !m_arrayBufferContentsArray->isEmpty();
}
