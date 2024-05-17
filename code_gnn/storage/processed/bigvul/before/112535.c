void Document::reportMemoryUsage(MemoryObjectInfo* memoryObjectInfo) const
{
    MemoryClassInfo info(memoryObjectInfo, this, WebCoreMemoryTypes::DOM);
    ContainerNode::reportMemoryUsage(memoryObjectInfo);
    TreeScope::reportMemoryUsage(memoryObjectInfo);
    ScriptExecutionContext::reportMemoryUsage(memoryObjectInfo);
    info.addMember(m_styleResolver, "styleResolver");
    info.addMember(m_url, "url");
    info.addMember(m_baseURL, "baseURL");
    info.addMember(m_baseURLOverride, "baseURLOverride");
    info.addMember(m_baseElementURL, "baseElementURL");
    info.addMember(m_cookieURL, "cookieURL");
    info.addMember(m_firstPartyForCookies, "firstPartyForCookies");
    info.addMember(m_documentURI, "documentURI");
    info.addMember(m_baseTarget, "baseTarget");
    info.addMember(m_docType, "docType");
    info.addMember(m_implementation, "implementation");
    info.addMember(m_elemSheet, "elemSheet");
    info.addMember(m_frame, "frame");
    info.addMember(m_cachedResourceLoader, "cachedResourceLoader");
    info.addMember(m_styleSheetCollection, "styleSheetCollection");
    info.addMember(m_styleSheetList, "styleSheetList");
    info.addMember(m_formController, "formController");
    info.addMember(m_nodeIterators, "nodeIterators");
    info.addMember(m_ranges, "ranges");
    info.addMember(m_title.string(), "title.string()");
    info.addMember(m_rawTitle.string(), "rawTitle.string()");
    info.addMember(m_xmlEncoding, "xmlEncoding");
    info.addMember(m_xmlVersion, "xmlVersion");
    info.addMember(m_contentLanguage, "contentLanguage");
#if ENABLE(DASHBOARD_SUPPORT) || ENABLE(DRAGGABLE_REGION)
    info.addMember(m_annotatedRegions, "annotatedRegions");
#endif
    info.addMember(m_cssCanvasElements, "cssCanvasElements");
    info.addMember(m_iconURLs, "iconURLs");
    info.addMember(m_documentSuspensionCallbackElements, "documentSuspensionCallbackElements");
    info.addMember(m_mediaVolumeCallbackElements, "mediaVolumeCallbackElements");
    info.addMember(m_privateBrowsingStateChangedElements, "privateBrowsingStateChangedElements");
    info.addMember(m_elementsByAccessKey, "elementsByAccessKey");
    info.addMember(m_eventQueue, "eventQueue");
    info.addMember(m_mediaCanStartListeners, "mediaCanStartListeners");
    info.addMember(m_pendingTasks, "pendingTasks");
#if ENABLE(LINK_PRERENDER)
    info.addMember(m_prerenderer, "prerenderer");
#endif
    info.addMember(m_listsInvalidatedAtDocument, "listsInvalidatedAtDocument");
    info.addMember(m_styleResolverThrowawayTimer, "styleResolverThrowawayTimer");
    info.addMember(m_domWindow, "domWindow");
    info.addMember(m_parser, "parser");
    info.addMember(m_contextFeatures, "contextFeatures");
    info.addMember(m_focusedNode, "focusedNode");
    info.addMember(m_hoverNode, "hoverNode");
    info.addMember(m_documentElement, "documentElement");
    info.addMember(m_visitedLinkState, "visitedLinkState");
    info.addMember(m_styleRecalcTimer, "styleRecalcTimer");
    info.addMember(m_titleElement, "titleElement");
    info.ignoreMember(m_renderArena);
    info.addMember(m_axObjectCache, "axObjectCache");
    info.addMember(m_markers, "markers");
    info.addMember(m_cssTarget, "cssTarget");
    info.addMember(m_updateFocusAppearanceTimer, "updateFocusAppearanceTimer");
    info.addMember(m_pendingStateObject, "pendingStateObject");
    info.addMember(m_scriptRunner, "scriptRunner");
#if ENABLE(XSLT)
    info.addMember(m_transformSource, "transformSource");
    info.addMember(m_transformSourceDocument, "transformSourceDocument");
#endif
    info.addMember(m_savedRenderer, "savedRenderer");
    info.addMember(m_decoder, "decoder");
    info.addMember(m_xpathEvaluator, "xpathEvaluator");
#if ENABLE(SVG)
    info.addMember(m_svgExtensions, "svgExtensions");
#endif
    info.addMember(m_selectorQueryCache, "selectorQueryCache");
    info.addMember(m_renderer, "renderer");
    info.addMember(m_weakFactory, "weakFactory");
    info.addMember(m_idAttributeName, "idAttributeName");
#if ENABLE(FULLSCREEN_API)
    info.addMember(m_fullScreenElement, "fullScreenElement");
    info.addMember(m_fullScreenElementStack, "fullScreenElementStack");
    info.addMember(m_fullScreenRenderer, "fullScreenRenderer");
    info.addMember(m_fullScreenChangeDelayTimer, "fullScreenChangeDelayTimer");
    info.addMember(m_fullScreenChangeEventTargetQueue, "fullScreenChangeEventTargetQueue");
    info.addMember(m_fullScreenErrorEventTargetQueue, "fullScreenErrorEventTargetQueue");
    info.addMember(m_savedPlaceholderRenderStyle, "savedPlaceholderRenderStyle");
#endif
#if ENABLE(DIALOG_ELEMENT)
    info.addMember(m_topLayerElements, "topLayerElements");
#endif
    info.addMember(m_loadEventDelayTimer, "loadEventDelayTimer");
    info.addMember(m_viewportArguments, "viewportArguments");
    info.addMember(m_documentTiming, "documentTiming");
    info.addMember(m_mediaQueryMatcher, "mediaQueryMatcher");
#if ENABLE(TOUCH_EVENTS)
    info.addMember(m_touchEventTargets, "touchEventTargets");
#endif
#if ENABLE(REQUEST_ANIMATION_FRAME)
    info.addMember(m_scriptedAnimationController, "scriptedAnimationController");
#endif
    info.addMember(m_pendingTasksTimer, "pendingTasksTimer");
#if ENABLE(TEXT_AUTOSIZING)
    info.addMember(m_textAutosizer, "textAutosizer");
#endif
    info.addMember(m_visualUpdatesSuppressionTimer, "visualUpdatesSuppressionTimer");
    info.addMember(m_namedFlows, "namedFlows");
#if ENABLE(CSP_NEXT)
    info.addMember(m_domSecurityPolicy, "domSecurityPolicy");
#endif
    info.addMember(m_sharedObjectPoolClearTimer, "sharedObjectPoolClearTimer");
    info.addMember(m_sharedObjectPool, "sharedObjectPool");
    info.addMember(m_localeCache, "localeCache");
#if ENABLE(TEMPLATE_ELEMENT)
    info.addMember(m_templateDocument, "templateDocument");
    info.addMember(m_templateDocumentHost, "templateDocumentHost");
#endif
    info.addMember(m_activeElement, "activeElement");
}
