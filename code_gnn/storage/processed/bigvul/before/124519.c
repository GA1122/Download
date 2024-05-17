unsigned RenderBlock::columnCount(ColumnInfo* colInfo) const
{
    ASSERT(hasColumns());
    ASSERT(gColumnInfoMap->get(this) == colInfo);
    return colInfo->columnCount();
}
