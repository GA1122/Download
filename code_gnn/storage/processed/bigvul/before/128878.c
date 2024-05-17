bool SVGDocumentExtensions::zoomAndPanEnabled() const
{
    if (SVGSVGElement* svg = rootElement(*m_document)) {
        if (svg->useCurrentView()) {
            if (svg->currentView())
                return svg->currentView()->zoomAndPan() == SVGZoomAndPanMagnify;
        } else {
            return svg->zoomAndPan() == SVGZoomAndPanMagnify;
        }
    }

    return false;
}
