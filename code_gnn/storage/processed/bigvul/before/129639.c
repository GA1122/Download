bool SVGLayoutSupport::willIsolateBlendingDescendantsForStyle(const ComputedStyle& style)
{
    const SVGComputedStyle& svgStyle = style.svgStyle();

    return style.hasIsolation() || style.opacity() < 1 || style.hasBlendMode()
        || style.hasFilter() || svgStyle.hasMasker() || svgStyle.hasClipper();
}
