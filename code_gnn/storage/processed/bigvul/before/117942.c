void ScriptController::collectGarbage()
{
    v8::HandleScope handleScope;

    v8::Persistent<v8::Context> v8Context = v8::Context::New();
    if (v8Context.IsEmpty())
        return;
    {
        v8::Context::Scope scope(v8Context);
        v8::Local<v8::String> source = v8::String::New("if (gc) gc();");
        v8::Local<v8::String> name = v8::String::New("gc");
        v8::Handle<v8::Script> script = v8::Script::Compile(source, name);
        if (!script.IsEmpty()) {
            V8RecursionScope::MicrotaskSuppression scope;
            script->Run();
        }
    }
    v8Context.Dispose();
}
