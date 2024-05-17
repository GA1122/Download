    void testResizeYieldsCorrectScrollAndScale(const char* url,
                                               const float initialPageScaleFactor,
                                               const WebSize scrollOffset,
                                               const WebSize viewportSize,
                                               const bool shouldScaleRelativeToViewportWidth) {
        UseMockScrollbarSettings mockScrollbarSettings;
        registerMockedHttpURLLoad(url);

        const float aspectRatio = static_cast<float>(viewportSize.width) / viewportSize.height;

        FrameTestHelpers::WebViewHelper webViewHelper;
        webViewHelper.initializeAndLoad(m_baseURL + url, true, 0, 0, enableViewportSettings);

        {
            webViewHelper.webViewImpl()->resize(WebSize(viewportSize.width, viewportSize.height));
            webViewHelper.webViewImpl()->setPageScaleFactor(initialPageScaleFactor, WebPoint());
            ASSERT_EQ(viewportSize, webViewHelper.webViewImpl()->size());
            ASSERT_EQ(initialPageScaleFactor, webViewHelper.webViewImpl()->pageScaleFactor());
            webViewHelper.webViewImpl()->resize(WebSize(viewportSize.height, viewportSize.width));
            float expectedPageScaleFactor = initialPageScaleFactor * (shouldScaleRelativeToViewportWidth ? 1 / aspectRatio : 1);
            EXPECT_NEAR(expectedPageScaleFactor, webViewHelper.webViewImpl()->pageScaleFactor(), 0.05f);
            EXPECT_EQ(WebSize(), webViewHelper.webViewImpl()->mainFrame()->scrollOffset());
        }

        {
            webViewHelper.webViewImpl()->resize(WebSize(viewportSize.width, viewportSize.height));
            webViewHelper.webViewImpl()->setPageScaleFactor(initialPageScaleFactor, WebPoint(scrollOffset.width, scrollOffset.height));
            webViewHelper.webViewImpl()->layout();
            const WebSize expectedScrollOffset = webViewHelper.webViewImpl()->mainFrame()->scrollOffset();
            webViewHelper.webViewImpl()->resize(WebSize(viewportSize.width, viewportSize.height * 0.8f));
            EXPECT_EQ(initialPageScaleFactor, webViewHelper.webViewImpl()->pageScaleFactor());
            EXPECT_EQ(expectedScrollOffset, webViewHelper.webViewImpl()->mainFrame()->scrollOffset());
            webViewHelper.webViewImpl()->resize(WebSize(viewportSize.width, viewportSize.height * 0.8f));
            EXPECT_EQ(initialPageScaleFactor, webViewHelper.webViewImpl()->pageScaleFactor());
            EXPECT_EQ(expectedScrollOffset, webViewHelper.webViewImpl()->mainFrame()->scrollOffset());
        }

        {
            webViewHelper.webViewImpl()->resize(WebSize(viewportSize.height, viewportSize.width));
            float pageScaleFactor = webViewHelper.webViewImpl()->pageScaleFactor();
            webViewHelper.webViewImpl()->resize(WebSize(viewportSize.width, viewportSize.height));
            float expectedPageScaleFactor = pageScaleFactor * (shouldScaleRelativeToViewportWidth ? aspectRatio : 1);
            EXPECT_NEAR(expectedPageScaleFactor, webViewHelper.webViewImpl()->pageScaleFactor(), 0.05f);
            webViewHelper.webViewImpl()->mainFrame()->setScrollOffset(scrollOffset);

            WebCore::IntPoint anchorPoint = WebCore::IntPoint(scrollOffset) + WebCore::IntPoint(viewportSize.width / 2, 0);
            RefPtr<WebCore::Node> anchorNode = webViewHelper.webViewImpl()->mainFrameImpl()->frame()->eventHandler().hitTestResultAtPoint(anchorPoint, HitTestRequest::ReadOnly | HitTestRequest::Active | HitTestRequest::ConfusingAndOftenMisusedDisallowShadowContent).innerNode();
            ASSERT(anchorNode);

            pageScaleFactor = webViewHelper.webViewImpl()->pageScaleFactor();
            const WebCore::FloatSize preResizeRelativeOffset
                = computeRelativeOffset(anchorPoint, anchorNode->boundingBox());
            webViewHelper.webViewImpl()->resize(WebSize(viewportSize.height, viewportSize.width));
            WebCore::IntPoint newAnchorPoint = WebCore::IntPoint(webViewHelper.webViewImpl()->mainFrame()->scrollOffset()) + WebCore::IntPoint(viewportSize.height / 2, 0);
            const WebCore::FloatSize postResizeRelativeOffset
                = computeRelativeOffset(newAnchorPoint, anchorNode->boundingBox());
            EXPECT_NEAR(preResizeRelativeOffset.width(), postResizeRelativeOffset.width(), 0.15f);
            expectedPageScaleFactor = pageScaleFactor * (shouldScaleRelativeToViewportWidth ? 1 / aspectRatio : 1);
            EXPECT_NEAR(expectedPageScaleFactor, webViewHelper.webViewImpl()->pageScaleFactor(), 0.05f);
        }
    }
