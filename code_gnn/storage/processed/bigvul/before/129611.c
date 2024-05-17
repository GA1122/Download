void SVGLayoutSupport::applyStrokeStyleToStrokeData(StrokeData& strokeData, const ComputedStyle& style, const LayoutObject& object, float dashScaleFactor)
{
    ASSERT(object.node());
    ASSERT(object.node()->isSVGElement());

    const SVGComputedStyle& svgStyle = style.svgStyle();

    SVGLengthContext lengthContext(toSVGElement(object.node()));
    strokeData.setThickness(lengthContext.valueForLength(svgStyle.strokeWidth()));
    strokeData.setLineCap(svgStyle.capStyle());
    strokeData.setLineJoin(svgStyle.joinStyle());
    strokeData.setMiterLimit(svgStyle.strokeMiterLimit());

    DashArray dashArray = resolveSVGDashArray(*svgStyle.strokeDashArray(), style, lengthContext);
    float dashOffset = lengthContext.valueForLength(svgStyle.strokeDashOffset(), style);
    if (dashScaleFactor != 1) {
        ASSERT(dashScaleFactor >= 0);
        dashOffset *= dashScaleFactor;
        for (auto& dashItem : dashArray)
            dashItem *= dashScaleFactor;
    }
    strokeData.setLineDash(dashArray, dashOffset);
}
