Document::Document(Frame* frame, const KURL& url, bool isXHTML, bool isHTML)
    : ContainerNode(0, CreateDocument)
    , TreeScope(this)
    , m_styleResolverThrowawayTimer(this, &Document::styleResolverThrowawayTimerFired)
    , m_lastStyleResolverAccessTime(0)
    , m_activeParserCount(0)
    , m_contextFeatures(ContextFeatures::defaultSwitch())
    , m_compatibilityMode(NoQuirksMode)
    , m_compatibilityModeLocked(false)
    , m_domTreeVersion(++s_globalTreeVersion)
    , m_mutationObserverTypes(0)
    , m_styleSheetCollection(DocumentStyleSheetCollection::create(this))
    , m_visitedLinkState(VisitedLinkState::create(this))
    , m_readyState(Complete)
    , m_styleRecalcTimer(this, &Document::styleRecalcTimerFired)
    , m_pendingStyleRecalcShouldForce(false)
    , m_frameElementsShouldIgnoreScrolling(false)
    , m_containsValidityStyleRules(false)
    , m_updateFocusAppearanceRestoresSelection(false)
    , m_ignoreDestructiveWriteCount(0)
    , m_titleSetExplicitly(false)
    , m_updateFocusAppearanceTimer(this, &Document::updateFocusAppearanceTimerFired)
    , m_loadEventFinished(false)
    , m_startTime(currentTime())
    , m_overMinimumLayoutThreshold(false)
    , m_scriptRunner(ScriptRunner::create(this))
    , m_xmlVersion("1.0")
    , m_xmlStandalone(StandaloneUnspecified)
    , m_hasXMLDeclaration(0)
    , m_savedRenderer(0)
    , m_designMode(inherit)
#if ENABLE(DASHBOARD_SUPPORT) || ENABLE(DRAGGABLE_REGION)
    , m_hasAnnotatedRegions(false)
    , m_annotatedRegionsDirty(false)
#endif
    , m_createRenderers(true)
    , m_inPageCache(false)
    , m_accessKeyMapValid(false)
    , m_useSecureKeyboardEntryWhenActive(false)
    , m_isXHTML(isXHTML)
    , m_isHTML(isHTML)
    , m_isViewSource(false)
    , m_sawElementsInKnownNamespaces(false)
    , m_isSrcdocDocument(false)
    , m_renderer(0)
    , m_eventQueue(DocumentEventQueue::create(this))
    , m_weakFactory(this)
    , m_idAttributeName(idAttr)
#if ENABLE(FULLSCREEN_API)
    , m_areKeysEnabledInFullScreen(0)
    , m_fullScreenRenderer(0)
    , m_fullScreenChangeDelayTimer(this, &Document::fullScreenChangeDelayTimerFired)
    , m_isAnimatingFullScreen(false)
#endif
    , m_loadEventDelayCount(0)
    , m_loadEventDelayTimer(this, &Document::loadEventDelayTimerFired)
    , m_referrerPolicy(ReferrerPolicyDefault)
    , m_directionSetOnDocumentElement(false)
    , m_writingModeSetOnDocumentElement(false)
    , m_writeRecursionIsTooDeep(false)
    , m_writeRecursionDepth(0)
    , m_wheelEventHandlerCount(0)
    , m_lastHandledUserGestureTimestamp(0)
    , m_pendingTasksTimer(this, &Document::pendingTasksTimerFired)
    , m_scheduledTasksAreSuspended(false)
    , m_visualUpdatesAllowed(true)
    , m_visualUpdatesSuppressionTimer(this, &Document::visualUpdatesSuppressionTimerFired)
    , m_sharedObjectPoolClearTimer(this, &Document::sharedObjectPoolClearTimerFired)
#ifndef NDEBUG
    , m_didDispatchViewportPropertiesChanged(false)
#endif
#if ENABLE(TEMPLATE_ELEMENT)
    , m_templateDocumentHost(0)
#endif
#if ENABLE(FONT_LOAD_EVENTS)
    , m_fontloader(0)
#endif
    , m_didAssociateFormControlsTimer(this, &Document::didAssociateFormControlsTimerFired)
{
    m_printing = false;
    m_paginatedForScreen = false;

    m_ignoreAutofocus = false;

    m_frame = frame;
    if (m_frame)
        provideContextFeaturesToDocumentFrom(this, m_frame->page());

    if ((frame && frame->ownerElement()) || !url.isEmpty())
        setURL(url);

    m_markers = adoptPtr(new DocumentMarkerController);

    if (m_frame)
        m_cachedResourceLoader = m_frame->loader()->activeDocumentLoader()->cachedResourceLoader();
    if (!m_cachedResourceLoader)
        m_cachedResourceLoader = CachedResourceLoader::create(0);
    m_cachedResourceLoader->setDocument(this);

#if ENABLE(LINK_PRERENDER)
    m_prerenderer = Prerenderer::create(this);
#endif
#if ENABLE(TEXT_AUTOSIZING)
    m_textAutosizer = TextAutosizer::create(this);
#endif
    m_visuallyOrdered = false;
    m_bParsing = false;
    m_wellFormed = false;

    m_textColor = Color::black;
    m_listenerTypes = 0;
    m_inStyleRecalc = false;
    m_closeAfterStyleRecalc = false;

    m_gotoAnchorNeededAfterStylesheetsLoad = false;

    m_didCalculateStyleResolver = false;
    m_ignorePendingStylesheets = false;
    m_needsNotifyRemoveAllPendingStylesheet = false;
    m_hasNodesWithPlaceholderStyle = false;
    m_pendingSheetLayout = NoLayoutWithPendingSheets;

    m_cssTarget = 0;

    resetLinkColor();
    resetVisitedLinkColor();
    resetActiveLinkColor();

    m_processingLoadEvent = false;
    
    initSecurityContext();
    initDNSPrefetch();

    static int docID = 0;
    m_docID = docID++;

    for (unsigned i = 0; i < WTF_ARRAY_LENGTH(m_nodeListCounts); i++)
        m_nodeListCounts[i] = 0;

    InspectorCounters::incrementCounter(InspectorCounters::DocumentCounter);
}
