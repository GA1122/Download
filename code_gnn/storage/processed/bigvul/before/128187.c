ScriptValue Notification::data(ScriptState* scriptState)
{
    if (m_developerData.isEmpty()) {
        RefPtr<SerializedScriptValue> serializedValue;

        const WebVector<char>& serializedData = m_data.data;
        if (serializedData.size())
            serializedValue = SerializedScriptValueFactory::instance().createFromWireBytes(serializedData.data(), serializedData.size());
        else
            serializedValue = SerializedScriptValueFactory::instance().create();

        m_developerData = ScriptValue(scriptState, serializedValue->deserialize(scriptState->isolate()));
    }

    return m_developerData;
}
