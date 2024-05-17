v8::Handle<v8::Script> V8Proxy::compileScript(v8::Handle<v8::String> code, const String& fileName, const TextPosition& scriptStartPosition, v8::ScriptData* scriptData)
{
    const uint16_t* fileNameString = fromWebCoreString(fileName);
    v8::Handle<v8::String> name = v8::String::New(fileNameString, fileName.length());
    v8::Handle<v8::Integer> line = v8::Integer::New(scriptStartPosition.m_line.zeroBasedInt());
    v8::Handle<v8::Integer> column = v8::Integer::New(scriptStartPosition.m_column.zeroBasedInt());
    v8::ScriptOrigin origin(name, line, column);
    v8::Handle<v8::Script> script = v8::Script::Compile(code, &origin, scriptData);
    return script;
}
