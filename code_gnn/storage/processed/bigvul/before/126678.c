  TabContents* CreateTabContentsWithSharedRPH(WebContents* web_contents) {
    TabContents* retval = chrome::TabContentsFactory(profile(),
        web_contents->GetRenderViewHost()->GetSiteInstance(), MSG_ROUTING_NONE,
        NULL);
    EXPECT_EQ(retval->web_contents()->GetRenderProcessHost(),
              web_contents->GetRenderProcessHost());
    return retval;
  }
