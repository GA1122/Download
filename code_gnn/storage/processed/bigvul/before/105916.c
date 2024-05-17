static const HashTable* getJSFloat64ArrayTable(ExecState* exec)
{
    return getHashTableForGlobalData(exec->globalData(), &JSFloat64ArrayTable);
}
