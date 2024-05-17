void V8Debugger::v8DebugEventCallback(const v8::Debug::EventDetails& eventDetails)
{
    V8Debugger* thisPtr = toV8Debugger(eventDetails.GetCallbackData());
    thisPtr->handleV8DebugEvent(eventDetails);
}
