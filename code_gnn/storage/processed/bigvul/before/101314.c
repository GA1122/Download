void SessionModelAssociator::AppendSessionTabNavigation(
    const sync_pb::TabNavigation& specifics,
    std::vector<TabNavigation>* navigations) {
  int index = 0;
  GURL virtual_url;
  GURL referrer;
  string16 title;
  std::string state;
  PageTransition::Type transition(PageTransition::LINK);
  if (specifics.has_index())
    index = specifics.index();
  if (specifics.has_virtual_url()) {
    GURL gurl(specifics.virtual_url());
    virtual_url = gurl;
  }
  if (specifics.has_referrer()) {
    GURL gurl(specifics.referrer());
    referrer = gurl;
  }
  if (specifics.has_title())
    title = UTF8ToUTF16(specifics.title());
  if (specifics.has_state())
    state = specifics.state();
  if (specifics.has_page_transition() ||
      specifics.has_navigation_qualifier()) {
    switch (specifics.page_transition()) {
      case sync_pb::TabNavigation_PageTransition_LINK:
        transition = PageTransition::LINK;
        break;
      case sync_pb::TabNavigation_PageTransition_TYPED:
        transition = PageTransition::TYPED;
        break;
      case sync_pb::TabNavigation_PageTransition_AUTO_BOOKMARK:
        transition = PageTransition::AUTO_BOOKMARK;
        break;
      case sync_pb::TabNavigation_PageTransition_AUTO_SUBFRAME:
        transition = PageTransition::AUTO_SUBFRAME;
        break;
      case sync_pb::TabNavigation_PageTransition_MANUAL_SUBFRAME:
        transition = PageTransition::MANUAL_SUBFRAME;
        break;
      case sync_pb::TabNavigation_PageTransition_GENERATED:
        transition = PageTransition::GENERATED;
        break;
      case sync_pb::TabNavigation_PageTransition_START_PAGE:
        transition = PageTransition::START_PAGE;
        break;
      case sync_pb::TabNavigation_PageTransition_FORM_SUBMIT:
        transition = PageTransition::FORM_SUBMIT;
        break;
      case sync_pb::TabNavigation_PageTransition_RELOAD:
        transition = PageTransition::RELOAD;
        break;
      case sync_pb::TabNavigation_PageTransition_KEYWORD:
        transition = PageTransition::KEYWORD;
        break;
      case sync_pb::TabNavigation_PageTransition_KEYWORD_GENERATED:
        transition = PageTransition::KEYWORD_GENERATED;
        break;
      case sync_pb::TabNavigation_PageTransition_CHAIN_START:
        transition = sync_pb::TabNavigation_PageTransition_CHAIN_START;
        break;
      case sync_pb::TabNavigation_PageTransition_CHAIN_END:
        transition = PageTransition::CHAIN_END;
        break;
      default:
        switch (specifics.navigation_qualifier()) {
          case sync_pb::
              TabNavigation_PageTransitionQualifier_CLIENT_REDIRECT:
            transition = PageTransition::CLIENT_REDIRECT;
            break;
            case sync_pb::
                TabNavigation_PageTransitionQualifier_SERVER_REDIRECT:
            transition = PageTransition::SERVER_REDIRECT;
              break;
            default:
            transition = PageTransition::TYPED;
        }
    }
  }
  TabNavigation tab_navigation(index, virtual_url, referrer, title, state,
                               transition);
  navigations->insert(navigations->end(), tab_navigation);
}
