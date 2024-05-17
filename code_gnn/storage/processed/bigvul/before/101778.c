void Browser::RegisterProtocolHandlerHelper(TabContents* tab,
                                            const std::string& protocol,
                                            const GURL& url,
                                            const string16& title) {
  TabContentsWrapper* tcw = TabContentsWrapper::GetCurrentWrapperForContents(
      tab);
  if (!tcw || tcw->profile()->IsOffTheRecord())
    return;

  ChildProcessSecurityPolicy* policy =
      ChildProcessSecurityPolicy::GetInstance();
  if (policy->IsPseudoScheme(protocol) || policy->IsDisabledScheme(protocol))
    return;

  ProtocolHandler handler =
      ProtocolHandler::CreateProtocolHandler(protocol, url, title);

  ProtocolHandlerRegistry* registry =
      tcw->profile()->GetProtocolHandlerRegistry();
  if (!registry->enabled() || registry->IsRegistered(handler) ||
      registry->IsIgnored(handler))
    return;

  if (!handler.IsEmpty() &&
      registry->CanSchemeBeOverridden(handler.protocol())) {
    UserMetrics::RecordAction(
        UserMetricsAction("RegisterProtocolHandler.InfoBar_Shown"));
    tcw->infobar_tab_helper()->AddInfoBar(
        new RegisterProtocolHandlerInfoBarDelegate(tab, registry, handler));
  }
}
