bool ScriptController::canAccessFromCurrentOrigin(Frame *frame)
{
    return !v8::Context::InContext() || V8BindingSecurity::canAccessFrame(V8BindingState::Only(), frame, true);
}
