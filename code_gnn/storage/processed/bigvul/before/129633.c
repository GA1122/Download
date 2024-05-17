DashArray SVGLayoutSupport::resolveSVGDashArray(const SVGDashArray& svgDashArray, const ComputedStyle& style, const SVGLengthContext& lengthContext)
{
    DashArray dashArray;
    for (const Length& dashLength : svgDashArray.vector())
        dashArray.append(lengthContext.valueForLength(dashLength, style));
    return dashArray;
}
