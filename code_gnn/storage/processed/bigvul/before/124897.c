static void markBoxForRelayoutAfterSplit(RenderBox* box)
{
    if (box->isTable()) {
        toRenderTable(box)->forceSectionsRecalc();
    } else if (box->isTableSection())
        toRenderTableSection(box)->setNeedsCellRecalc();

    box->setNeedsLayoutAndPrefWidthsRecalc();
}
