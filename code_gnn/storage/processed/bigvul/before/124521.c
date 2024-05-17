ColumnInfo* RenderBlock::columnInfo() const
{
    if (!hasColumns())
        return 0;
    return gColumnInfoMap->get(this);
}
