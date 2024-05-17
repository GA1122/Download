void SVGDocumentExtensions::serviceOnAnimationFrame(Document& document, double monotonicAnimationStartTime)
{
    if (!document.svgExtensions())
        return;
    document.accessSVGExtensions().serviceAnimations(monotonicAnimationStartTime);
}
