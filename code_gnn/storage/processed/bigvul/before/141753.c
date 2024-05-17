bool V8Debugger::setScriptSource(const String16& sourceID, v8::Local<v8::String> newSource, bool dryRun, ErrorString* error, Maybe<protocol::Runtime::ExceptionDetails>* exceptionDetails, JavaScriptCallFrames* newCallFrames, Maybe<bool>* stackChanged)
{
    class EnableLiveEditScope {
    public:
        explicit EnableLiveEditScope(v8::Isolate* isolate) : m_isolate(isolate)
        {
            v8::Debug::SetLiveEditEnabled(m_isolate, true);
            inLiveEditScope = true;
        }
        ~EnableLiveEditScope()
        {
            v8::Debug::SetLiveEditEnabled(m_isolate, false);
            inLiveEditScope = false;
        }
    private:
        v8::Isolate* m_isolate;
    };

    DCHECK(enabled());
    v8::HandleScope scope(m_isolate);

    std::unique_ptr<v8::Context::Scope> contextScope;
    if (!isPaused())
        contextScope = wrapUnique(new v8::Context::Scope(debuggerContext()));

    v8::Local<v8::Value> argv[] = { toV8String(m_isolate, sourceID), newSource, v8Boolean(dryRun, m_isolate) };

    v8::Local<v8::Value> v8result;
    {
        EnableLiveEditScope enableLiveEditScope(m_isolate);
        v8::TryCatch tryCatch(m_isolate);
        tryCatch.SetVerbose(false);
        v8::MaybeLocal<v8::Value> maybeResult = callDebuggerMethod("liveEditScriptSource", 3, argv);
        if (tryCatch.HasCaught()) {
            v8::Local<v8::Message> message = tryCatch.Message();
            if (!message.IsEmpty())
                *error = toProtocolStringWithTypeCheck(message->Get());
            else
                *error = "Unknown error.";
            return false;
        }
        v8result = maybeResult.ToLocalChecked();
    }
    DCHECK(!v8result.IsEmpty());
    v8::Local<v8::Object> resultTuple = v8result->ToObject(m_isolate);
    int code = static_cast<int>(resultTuple->Get(0)->ToInteger(m_isolate)->Value());
    switch (code) {
    case 0:
        {
            *stackChanged = resultTuple->Get(1)->BooleanValue();
            if (!dryRun && isPaused()) {
                JavaScriptCallFrames frames = currentCallFrames();
                newCallFrames->swap(frames);
            }
            return true;
        }
    case 1:
        {
            *exceptionDetails = protocol::Runtime::ExceptionDetails::create()
                .setText(toProtocolStringWithTypeCheck(resultTuple->Get(2)))
                .setScriptId(String16("0"))
                .setLineNumber(resultTuple->Get(3)->ToInteger(m_isolate)->Value() - 1)
                .setColumnNumber(resultTuple->Get(4)->ToInteger(m_isolate)->Value() - 1).build();
            return false;
        }
    }
    *error = "Unknown error.";
    return false;
}
