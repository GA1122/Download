void RenderBlock::setDesiredColumnCountAndWidth(int count, LayoutUnit width)
{
    bool destroyColumns = !requiresColumns(count);
    if (destroyColumns) {
        if (hasColumns()) {
            gColumnInfoMap->take(this);
            setHasColumns(false);
        }
    } else {
        ColumnInfo* info;
        if (hasColumns())
            info = gColumnInfoMap->get(this);
        else {
            if (!gColumnInfoMap)
                gColumnInfoMap = new ColumnInfoMap;
            info = new ColumnInfo;
            gColumnInfoMap->add(this, adoptPtr(info));
            setHasColumns(true);
        }
        info->setDesiredColumnWidth(width);
        if (style()->isOverflowPaged()) {
            info->setDesiredColumnCount(1);
            info->setProgressionAxis(style()->hasInlinePaginationAxis() ? ColumnInfo::InlineAxis : ColumnInfo::BlockAxis);
        } else {
            info->setDesiredColumnCount(count);
            info->setProgressionAxis(ColumnInfo::InlineAxis);
        }
    }
}
