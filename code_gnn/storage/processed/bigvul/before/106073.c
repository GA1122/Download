void JSTestObj::destroy(JSC::JSCell* cell)
{
    JSTestObj* thisObject = jsCast<JSTestObj*>(cell);
    thisObject->JSTestObj::~JSTestObj();
}
