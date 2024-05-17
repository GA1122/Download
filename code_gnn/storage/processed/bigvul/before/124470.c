void RenderBlock::absoluteQuads(Vector<FloatQuad>& quads, bool* wasFixed) const
{
    if (isAnonymousBlockContinuation()) {
        FloatRect localRect(0, -collapsedMarginBefore().toFloat(),
            width().toFloat(), (height() + collapsedMarginBefore() + collapsedMarginAfter()).toFloat());
        quads.append(localToAbsoluteQuad(localRect, 0  , wasFixed));
        continuation()->absoluteQuads(quads, wasFixed);
    } else {
        quads.append(RenderBox::localToAbsoluteQuad(FloatRect(0, 0, width().toFloat(), height().toFloat()), 0  , wasFixed));
    }
}
