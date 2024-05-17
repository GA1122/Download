void JSTestInterface::destroy(JSC::JSCell* cell)
{
    JSTestInterface* thisObject = jsCast<JSTestInterface*>(cell);
    thisObject->JSTestInterface::~JSTestInterface();
}
