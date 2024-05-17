JSClassRef QtBuiltinBundlePage::navigatorQtObjectClass()
{
    static JSClassRef classRef = 0;
    if (!classRef) {
        const JSClassDefinition navigatorQtObjectClass = kJSClassDefinitionEmpty;
        classRef = JSClassCreate(&navigatorQtObjectClass);
    }
    return classRef;
}
