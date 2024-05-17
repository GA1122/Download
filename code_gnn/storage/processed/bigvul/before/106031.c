void JSTestMediaQueryListListener::destroy(JSC::JSCell* cell)
{
    JSTestMediaQueryListListener* thisObject = jsCast<JSTestMediaQueryListListener*>(cell);
    thisObject->JSTestMediaQueryListListener::~JSTestMediaQueryListListener();
}
