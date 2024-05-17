static void didReceiveAuthenticationChallengeInFrame(WKPageRef page, WKFrameRef frame, WKAuthenticationChallengeRef authenticationChallenge, const void *clientInfo)
{
    webkitWebViewHandleAuthenticationChallenge(WEBKIT_WEB_VIEW(clientInfo), toImpl(authenticationChallenge));
}
