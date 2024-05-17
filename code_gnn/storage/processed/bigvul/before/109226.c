void drawOutlinedQuad(GraphicsContext* context, const FloatQuad& quad, const Color& fillColor, const Color& outlineColor)
{
    static const int outlineThickness = 2;

    Path quadPath = quadToPath(quad);

    {
        context->save();
        context->clipOut(quadPath);

        context->setStrokeThickness(outlineThickness);
        context->setStrokeColor(outlineColor);
        context->strokePath(quadPath);

        context->restore();
    }

    context->setFillColor(fillColor);
    context->fillPath(quadPath);
}
