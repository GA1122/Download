bool HTMLMediaElement::isMediaDataCORSSameOrigin(SecurityOrigin* origin) const {
  return hasSingleSecurityOrigin() &&
         ((webMediaPlayer() && webMediaPlayer()->didPassCORSAccessCheck()) ||
          !origin->taintsCanvas(currentSrc()));
}
