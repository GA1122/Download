bool GetGuestCallback(content::WebContents** guest_out,
                      content::WebContents* guest) {
  EXPECT_FALSE(*guest_out);
  *guest_out = guest;
  return false;
}
