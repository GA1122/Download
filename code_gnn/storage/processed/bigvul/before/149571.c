  void RunCorsTest(bool use_cors_for_preconnect,
                   bool use_cors_for_resource_request) {
    const char* kCrossOriginValue[]{
        "anonymous",
        "use-credentials",
    };

    ui_test_utils::NavigateToURL(
        browser(), preconnecting_test_server()->GetURL("/title1.html"));

    content::WebContents* tab =
        browser()->tab_strip_model()->GetActiveWebContents();
    GURL preconnect_url = embedded_test_server()->base_url();
    std::string start_preconnect = base::StringPrintf(
        "var link = document.createElement('link');"
        "link.rel = 'preconnect';"
        "link.crossOrigin = '%s';"
        "link.href = '%s';"
        "document.head.appendChild(link);",
        kCrossOriginValue[use_cors_for_preconnect],
        preconnect_url.spec().c_str());
    ASSERT_TRUE(content::ExecJs(tab, start_preconnect));
    connection_tracker()->WaitUntilFirstConnectionAccepted();
    EXPECT_EQ(1u, connection_tracker()->GetAcceptedSocketCount());
    EXPECT_EQ(0u, connection_tracker()->GetReadSocketCount());

    GURL image_url = embedded_test_server()->GetURL("/test.gif");
    std::string load_image = base::StringPrintf(
        "var image = document.createElement('img');"
        "image.crossOrigin = '%s';"
        "image.src = '%s';"
        "document.body.appendChild(image);",
        kCrossOriginValue[use_cors_for_resource_request],
        image_url.spec().c_str());
    ASSERT_TRUE(content::ExecJs(tab, load_image));
    connection_tracker()->WaitUntilFirstConnectionRead();

    if (use_cors_for_preconnect == use_cors_for_resource_request) {
      EXPECT_EQ(1u, connection_tracker()->GetAcceptedSocketCount());
      EXPECT_EQ(1u, connection_tracker()->GetReadSocketCount());
    } else {
      EXPECT_EQ(2u, connection_tracker()->GetAcceptedSocketCount());
      EXPECT_EQ(1u, connection_tracker()->GetReadSocketCount());
    }
  }
