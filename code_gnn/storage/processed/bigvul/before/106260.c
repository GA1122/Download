ConstructType JSTestSerializedScriptValueInterfaceConstructor::getConstructData(JSCell*, ConstructData& constructData)
{
    constructData.native.function = constructJSTestSerializedScriptValueInterface;
    return ConstructTypeHost;
}
