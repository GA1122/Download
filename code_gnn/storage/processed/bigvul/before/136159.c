ScriptValue Document::getCSSCanvasContext(ScriptState* scriptState, const String& type, const String& name, int width, int height)
{
    HTMLCanvasElement& element = getCSSCanvasElement(name);
    element.setSize(IntSize(width, height));
    CanvasRenderingContext* context = element.getCanvasRenderingContext(type, CanvasContextCreationAttributes());
    if (!context) {
        return ScriptValue::createNull(scriptState);
    }

    return ScriptValue(scriptState, toV8(context, scriptState->context()->Global(), scriptState->isolate()));
}
