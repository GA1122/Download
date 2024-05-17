ConstructType JSTestNamedConstructorNamedConstructor::getConstructData(JSCell*, ConstructData& constructData)
{
    constructData.native.function = constructJSTestNamedConstructor;
    return ConstructTypeHost;
}
