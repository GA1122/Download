void ScriptController::installFunctionsForPagePopup(Frame* frame, PagePopupClient* popupClient)
{
    ASSERT(frame);
    ASSERT(popupClient);
    v8::HandleScope handleScope;
    v8::Handle<v8::Context> context = V8Proxy::mainWorldContext(frame);
    if (context.IsEmpty()) {
        ASSERT_NOT_REACHED();
        return;
    }
    v8::Context::Scope scope(context);
    DOMWindowPagePopup::install(frame->existingDOMWindow(), popupClient);
    v8::Local<v8::FunctionTemplate> templ = v8::FunctionTemplate::New(setValueAndClosePopupCallback, V8DOMWindow::wrap(frame->existingDOMWindow()));
    context->Global()->Set(v8::String::New("setValueAndClosePopup"), v8::Handle<v8::Function>(templ->GetFunction()));
}
