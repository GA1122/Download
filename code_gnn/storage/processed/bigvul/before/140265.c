v8::Handle<v8::Value> DialogHandler::returnValue() const
{
    if (!m_scriptStateForDialogFrame)
        return v8Undefined();
    ASSERT(m_scriptStateForDialogFrame->contextIsValid());

    v8::Isolate* isolate = m_scriptStateForDialogFrame->isolate();
    v8::EscapableHandleScope handleScope(isolate);
    ScriptState::Scope scope(m_scriptStateForDialogFrame.get());
    v8::Local<v8::Value> returnValue = m_scriptStateForDialogFrame->context()->Global()->Get(v8AtomicString(isolate, "returnValue"));
    if (returnValue.IsEmpty())
        return v8Undefined();
    return handleScope.Escape(returnValue);
}
