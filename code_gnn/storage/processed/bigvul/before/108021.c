void InlineTextBox::paintCompositionUnderline(GraphicsContext* ctx, int tx, int ty, const CompositionUnderline& underline)
{
    tx += m_x;
    ty += m_y;

    if (m_truncation == cFullTruncation)
        return;
    
    int start = 0;                  
    int width = m_logicalWidth;            
    bool useWholeWidth = true;
    unsigned paintStart = m_start;
    unsigned paintEnd = end() + 1;  
    if (paintStart <= underline.startOffset) {
        paintStart = underline.startOffset;
        useWholeWidth = false;
        start = toRenderText(renderer())->width(m_start, paintStart - m_start, textPos(), m_firstLine);
    }
    if (paintEnd != underline.endOffset) {       
        paintEnd = min(paintEnd, (unsigned)underline.endOffset);
        useWholeWidth = false;
    }
    if (m_truncation != cNoTruncation) {
        paintEnd = min(paintEnd, (unsigned)m_start + m_truncation);
        useWholeWidth = false;
    }
    if (!useWholeWidth) {
        width = toRenderText(renderer())->width(paintStart, paintEnd - paintStart, textPos() + start, m_firstLine);
    }

    int lineThickness = 1;
    int baseline = renderer()->style(m_firstLine)->font().ascent();
    if (underline.thick && logicalHeight() - baseline >= 2)
        lineThickness = 2;

    start += 1;
    width -= 2;

    ctx->setStrokeColor(underline.color, renderer()->style()->colorSpace());
    ctx->setStrokeThickness(lineThickness);
    ctx->drawLineForText(IntPoint(tx + start, ty + logicalHeight() - lineThickness), width, textRenderer()->document()->printing());
}
