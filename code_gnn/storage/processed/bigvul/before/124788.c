void RenderBox::absoluteQuads(Vector<FloatQuad>& quads, bool* wasFixed) const
{
    quads.append(localToAbsoluteQuad(FloatRect(0, 0, width().toFloat(), height().toFloat()), 0  , wasFixed));
}
