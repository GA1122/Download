void RenderBlockFlow::clearFloats(EClear clear)
{
    positionNewFloats();
    LayoutUnit newY = 0;
    switch (clear) {
    case CLEFT:
        newY = lowestFloatLogicalBottom(FloatingObject::FloatLeft);
        break;
    case CRIGHT:
        newY = lowestFloatLogicalBottom(FloatingObject::FloatRight);
        break;
    case CBOTH:
        newY = lowestFloatLogicalBottom();
    default:
        break;
    }
    if (height() < newY)
        setLogicalHeight(newY);
}
