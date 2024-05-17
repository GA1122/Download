MarginInfo::MarginInfo(RenderBlockFlow* blockFlow, LayoutUnit beforeBorderPadding, LayoutUnit afterBorderPadding)
    : m_canCollapseMarginAfterWithLastChild(true)
    , m_atBeforeSideOfBlock(true)
    , m_atAfterSideOfBlock(false)
    , m_hasMarginBeforeQuirk(false)
    , m_hasMarginAfterQuirk(false)
    , m_determinedMarginBeforeQuirk(false)
    , m_discardMargin(false)
{
    RenderStyle* blockStyle = blockFlow->style();
    ASSERT(blockFlow->isRenderView() || blockFlow->parent());
    m_canCollapseWithChildren = !blockFlow->createsBlockFormattingContext() && !blockFlow->isRenderFlowThread() && !blockFlow->isRenderView();

    m_canCollapseMarginBeforeWithChildren = m_canCollapseWithChildren && !beforeBorderPadding && blockStyle->marginBeforeCollapse() != MSEPARATE;

    m_canCollapseMarginAfterWithChildren = m_canCollapseWithChildren && !afterBorderPadding
        && (blockStyle->logicalHeight().isAuto() && !blockStyle->logicalHeight().value()) && blockStyle->marginAfterCollapse() != MSEPARATE;

    m_quirkContainer = blockFlow->isTableCell() || blockFlow->isBody();

    m_discardMargin = m_canCollapseMarginBeforeWithChildren && blockFlow->mustDiscardMarginBefore();

    m_positiveMargin = (m_canCollapseMarginBeforeWithChildren && !blockFlow->mustDiscardMarginBefore()) ? blockFlow->maxPositiveMarginBefore() : LayoutUnit();
    m_negativeMargin = (m_canCollapseMarginBeforeWithChildren && !blockFlow->mustDiscardMarginBefore()) ? blockFlow->maxNegativeMarginBefore() : LayoutUnit();
}
