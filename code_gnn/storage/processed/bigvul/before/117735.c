bool fillTestEventConstructorInit(TestEventConstructorInit& eventInit, const Dictionary& options)
{
    options.get("attr2", eventInit.attr2);
    return true;
}
