ConstructType JSFloat64ArrayConstructor::getConstructData(JSCell*, ConstructData& constructData)
{
    constructData.native.function = constructJSFloat64Array;
    return ConstructTypeHost;
}
