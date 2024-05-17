RenderFrameHostImpl* PrepareToDuplicateHosts(Shell* shell,
                                             int* target_routing_id) {
  GURL foo("http://foo.com/simple_page.html");

  NavigateToURL(shell, foo);
  EXPECT_EQ(base::ASCIIToUTF16("OK"), shell->web_contents()->GetTitle());

  ShellAddedObserver shell2_observer;
  EXPECT_TRUE(ExecuteScript(shell, "window.open(document.URL + '#2');"));
  Shell* shell2 = shell2_observer.GetShell();

  EXPECT_EQ(shell->web_contents()->GetMainFrame()->GetProcess()->GetID(),
            shell2->web_contents()->GetMainFrame()->GetProcess()->GetID());
  *target_routing_id =
      shell2->web_contents()->GetRenderViewHost()->GetRoutingID();
  EXPECT_NE(*target_routing_id,
            shell->web_contents()->GetRenderViewHost()->GetRoutingID());

  GURL extension_url("https://bar.com/simple_page.html");
  WebContentsImpl* wc = static_cast<WebContentsImpl*>(shell->web_contents());
  wc->GetFrameTree()->root()->navigator()->RequestOpenURL(
      wc->GetFrameTree()->root()->current_frame_host(), extension_url, false,
      nullptr, std::string(), Referrer(), WindowOpenDisposition::CURRENT_TAB,
      false, false, true, blink::WebTriggeringEventInfo::kFromTrustedEvent);

  RenderFrameHostImpl* next_rfh;
  if (IsBrowserSideNavigationEnabled())
    next_rfh = wc->GetRenderManagerForTesting()->speculative_frame_host();
  else
    next_rfh = wc->GetRenderManagerForTesting()->pending_frame_host();

  EXPECT_TRUE(next_rfh);
  EXPECT_NE(shell->web_contents()->GetRenderProcessHost()->GetID(),
            next_rfh->GetProcess()->GetID());

  return next_rfh;
}
