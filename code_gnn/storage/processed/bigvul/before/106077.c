ConstructType JSTestObjConstructor::getConstructData(JSCell*, ConstructData& constructData)
{
    constructData.native.function = constructJSTestObj;
    return ConstructTypeHost;
}
