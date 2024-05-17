void WebPage::gestureWillBegin(const WebCore::IntPoint& point, bool& canBeginPanning)
{
    bool hitScrollbar = false;

    HitTestRequest request(HitTestRequest::ReadOnly);
    for (Frame* childFrame = m_page->mainFrame(); childFrame; childFrame = EventHandler::subframeForTargetNode(m_gestureTargetNode.get())) {
        ScrollView* scollView = childFrame->view();
        if (!scollView)
            break;
        
        RenderView* renderView = childFrame->document()->renderView();
        if (!renderView)
            break;

        RenderLayer* layer = renderView->layer();
        if (!layer)
            break;

        HitTestResult result = scollView->windowToContents(point);
        layer->hitTest(request, result);
        m_gestureTargetNode = result.innerNode();

        if (!hitScrollbar)
            hitScrollbar = result.scrollbar();
    }

    if (hitScrollbar) {
        canBeginPanning = false;
        return;
    }

    if (!m_gestureTargetNode) {
        canBeginPanning = false;
        return;
    }

    for (RenderObject* renderer = m_gestureTargetNode->renderer(); renderer; renderer = renderer->parent()) {
        if (renderer->isBox() && toRenderBox(renderer)->canBeScrolledAndHasScrollableArea()) {
            canBeginPanning = true;
            return;
        }
    }

     canBeginPanning = false;
 }
