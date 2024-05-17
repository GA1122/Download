V8LazyEventListener::V8LazyEventListener(const AtomicString& functionName, const AtomicString& eventParameterName, const String& code, const String sourceURL, const TextPosition& position, Node* node, v8::Isolate* isolate)
    : V8AbstractEventListener(true, DOMWrapperWorld::mainWorld(), isolate)
    , m_functionName(functionName)
    , m_eventParameterName(eventParameterName)
    , m_code(code)
    , m_sourceURL(sourceURL)
    , m_node(node)
    , m_position(position)
{
}
