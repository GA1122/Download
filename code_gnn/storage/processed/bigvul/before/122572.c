void WebDevToolsAgentImpl::paintPageOverlay(WebCanvas* canvas)
{
    InspectorController* ic = inspectorController();
    if (ic) {
        GraphicsContext context(canvas);
        context.setCertainlyOpaque(false);
        ic->drawHighlight(context);
    }
}
