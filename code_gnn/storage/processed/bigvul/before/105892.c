void StrictModeTypeErrorFunction::destroy(JSCell* cell)
{
    jsCast<StrictModeTypeErrorFunction*>(cell)->StrictModeTypeErrorFunction::~StrictModeTypeErrorFunction();
}
