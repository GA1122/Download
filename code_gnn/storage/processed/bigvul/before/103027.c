  TabContentsWrapper* CreateTabContentsWithSharedRPH(
      TabContents* tab_contents) {
    TabContentsWrapper* retval = Browser::TabContentsFactory(profile(),
        tab_contents->render_view_host()->site_instance(), MSG_ROUTING_NONE,
        NULL, NULL);
    EXPECT_EQ(retval->tab_contents()->GetRenderProcessHost(),
              tab_contents->GetRenderProcessHost());
    return retval;
  }
