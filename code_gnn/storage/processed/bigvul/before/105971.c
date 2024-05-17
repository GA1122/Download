void JSTestEventTarget::destroy(JSC::JSCell* cell)
{
    JSTestEventTarget* thisObject = jsCast<JSTestEventTarget*>(cell);
    thisObject->JSTestEventTarget::~JSTestEventTarget();
}
