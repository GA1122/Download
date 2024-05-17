void drawClippedRect(GraphicsContext& context, const TestDisplayItemClient& client, DisplayItem::Type clipType, DisplayItem::Type drawingType, const FloatRect& bound)
{
    ClipRecorder clipRecorder(context, client, clipType, LayoutRect(1, 1, 9, 9));
    drawRect(context, client, drawingType, bound);
}
