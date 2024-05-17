void FreePicture(Display* display, XID picture) {
  XRenderFreePicture(display, picture);
}
