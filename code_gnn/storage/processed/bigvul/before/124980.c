void RenderFlexibleBox::alignChildren(const Vector<LineContext>& lineContexts)
{
    Vector<LayoutUnit> minMarginAfterBaselines;

    RenderBox* child = m_orderIterator.first();
    for (size_t lineNumber = 0; lineNumber < lineContexts.size(); ++lineNumber) {
        LayoutUnit minMarginAfterBaseline = LayoutUnit::max();
        LayoutUnit lineCrossAxisExtent = lineContexts[lineNumber].crossAxisExtent;
        LayoutUnit maxAscent = lineContexts[lineNumber].maxAscent;

        for (size_t childNumber = 0; childNumber < lineContexts[lineNumber].numberOfChildren; ++childNumber, child = m_orderIterator.next()) {
            ASSERT(child);
            if (child->isOutOfFlowPositioned()) {
                if (style()->flexWrap() == FlexWrapReverse)
                    adjustAlignmentForChild(child, lineCrossAxisExtent);
                continue;
            }

            if (updateAutoMarginsInCrossAxis(child, std::max(LayoutUnit(0), availableAlignmentSpaceForChild(lineCrossAxisExtent, child))))
                continue;

            switch (alignmentForChild(child)) {
            case ItemPositionAuto:
                ASSERT_NOT_REACHED();
                break;
            case ItemPositionStretch: {
                applyStretchAlignmentToChild(child, lineCrossAxisExtent);
                if (style()->flexWrap() == FlexWrapReverse)
                    adjustAlignmentForChild(child, availableAlignmentSpaceForChild(lineCrossAxisExtent, child));
                break;
            }
            case ItemPositionFlexStart:
                break;
            case ItemPositionFlexEnd:
                adjustAlignmentForChild(child, availableAlignmentSpaceForChild(lineCrossAxisExtent, child));
                break;
            case ItemPositionCenter:
                adjustAlignmentForChild(child, availableAlignmentSpaceForChild(lineCrossAxisExtent, child) / 2);
                break;
            case ItemPositionBaseline: {
                LayoutUnit ascent = marginBoxAscentForChild(child);
                LayoutUnit startOffset = maxAscent - ascent;
                adjustAlignmentForChild(child, startOffset);

                if (style()->flexWrap() == FlexWrapReverse)
                    minMarginAfterBaseline = std::min(minMarginAfterBaseline, availableAlignmentSpaceForChild(lineCrossAxisExtent, child) - startOffset);
                break;
            }
            case ItemPositionSelfStart:
            case ItemPositionSelfEnd:
            case ItemPositionStart:
            case ItemPositionEnd:
            case ItemPositionLeft:
            case ItemPositionRight:
                ASSERT_NOT_REACHED();
                break;
            }
        }
        minMarginAfterBaselines.append(minMarginAfterBaseline);
    }

    if (style()->flexWrap() != FlexWrapReverse)
        return;

    child = m_orderIterator.first();
    for (size_t lineNumber = 0; lineNumber < lineContexts.size(); ++lineNumber) {
        LayoutUnit minMarginAfterBaseline = minMarginAfterBaselines[lineNumber];
        for (size_t childNumber = 0; childNumber < lineContexts[lineNumber].numberOfChildren; ++childNumber, child = m_orderIterator.next()) {
            ASSERT(child);
            if (alignmentForChild(child) == ItemPositionBaseline && !hasAutoMarginsInCrossAxis(child) && minMarginAfterBaseline)
                adjustAlignmentForChild(child, minMarginAfterBaseline);
        }
    }
}
