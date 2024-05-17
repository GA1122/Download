   void GoBackCrossSite() {
//   void GoBack(bool is_cross_site) {
      NavigationEntry* entry = contents()->controller().GetEntryAtOffset(-1);
      ASSERT_TRUE(entry);
      contents()->controller().GoBack();
  
    contents()->TestDidNavigate(
        contents()->pending_rvh(), entry->page_id(), GURL(entry->url()),
        content::PAGE_TRANSITION_TYPED);
//      
//     RenderViewHost* rvh = is_cross_site ?
//         contents()->pending_rvh() :
//         contents()->render_view_host();
//     contents()->TestDidNavigate(rvh, entry->page_id(), GURL(entry->url()),
//                                 content::PAGE_TRANSITION_TYPED);
    }