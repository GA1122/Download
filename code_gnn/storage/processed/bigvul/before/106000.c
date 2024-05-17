ConstructType JSTestInterfaceConstructor::getConstructData(JSCell*, ConstructData& constructData)
{
    constructData.native.function = constructJSTestInterface;
    return ConstructTypeHost;
}
