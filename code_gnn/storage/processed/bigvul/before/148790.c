static void InitInterstitialPageMap() {
  if (!g_web_contents_to_interstitial_page)
    g_web_contents_to_interstitial_page = new InterstitialPageMap;
}
