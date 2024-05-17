void InspectorOverlay::drawNodeHighlight()
{
    if (!m_highlightNode)
        return;

    Highlight highlight;
    buildNodeHighlight(m_highlightNode.get(), m_nodeHighlightConfig, &highlight);
    if (m_eventTargetNode) {
        Highlight eventTargetHighlight;
        buildNodeHighlight(m_eventTargetNode.get(), m_nodeHighlightConfig, &eventTargetHighlight);
        highlight.quads.append(eventTargetHighlight.quads[1]);  
    }
    RefPtr<JSONObject> highlightObject = buildObjectForHighlight(highlight);

    Node* node = m_highlightNode.get();
    if (node->isElementNode() && m_nodeHighlightConfig.showInfo && node->renderer() && node->document().frame()) {
        RefPtr<JSONObject> elementInfo = JSONObject::create();
        Element* element = toElement(node);
        Element* realElement = element;
        PseudoElement* pseudoElement = 0;
        if (element->isPseudoElement()) {
            pseudoElement = toPseudoElement(element);
            realElement = element->parentOrShadowHostElement();
        }
        bool isXHTML = realElement->document().isXHTMLDocument();
        elementInfo->setString("tagName", isXHTML ? realElement->nodeName() : realElement->nodeName().lower());
        elementInfo->setString("idValue", realElement->getIdAttribute());
        StringBuilder classNames;
        if (realElement->hasClass() && realElement->isStyledElement()) {
            HashSet<AtomicString> usedClassNames;
            const SpaceSplitString& classNamesString = realElement->classNames();
            size_t classNameCount = classNamesString.size();
            for (size_t i = 0; i < classNameCount; ++i) {
                const AtomicString& className = classNamesString[i];
                if (!usedClassNames.add(className).isNewEntry)
                    continue;
                classNames.append('.');
                classNames.append(className);
            }
        }
        if (pseudoElement) {
            if (pseudoElement->pseudoId() == BEFORE)
                classNames.append("::before");
            else if (pseudoElement->pseudoId() == AFTER)
                classNames.append("::after");
        }
        if (!classNames.isEmpty())
            elementInfo->setString("className", classNames.toString());

        RenderObject* renderer = node->renderer();
        Frame* containingFrame = node->document().frame();
        FrameView* containingView = containingFrame->view();
        IntRect boundingBox = pixelSnappedIntRect(containingView->contentsToRootView(renderer->absoluteBoundingBoxRect()));
        RenderBoxModelObject* modelObject = renderer->isBoxModelObject() ? toRenderBoxModelObject(renderer) : 0;
        elementInfo->setString("nodeWidth", String::number(modelObject ? adjustForAbsoluteZoom(modelObject->pixelSnappedOffsetWidth(), modelObject) : boundingBox.width()));
        elementInfo->setString("nodeHeight", String::number(modelObject ? adjustForAbsoluteZoom(modelObject->pixelSnappedOffsetHeight(), modelObject) : boundingBox.height()));
        highlightObject->setObject("elementInfo", elementInfo.release());
    }
    evaluateInOverlay("drawNodeHighlight", highlightObject);
}
