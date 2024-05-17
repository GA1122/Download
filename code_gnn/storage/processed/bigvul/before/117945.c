PassScriptInstance ScriptController::createScriptInstanceForWidget(Widget* widget)
{
    ASSERT(widget);

    if (widget->isFrameView())
        return 0;

    NPObject* npObject = PlatformSupport::pluginScriptableObject(widget);

    if (!npObject)
        return 0;


    v8::Local<v8::Object> wrapper = createV8ObjectForNPObject(npObject, 0);

    m_pluginObjects.set(widget, npObject);

    return V8ScriptInstance::create(wrapper);
}
