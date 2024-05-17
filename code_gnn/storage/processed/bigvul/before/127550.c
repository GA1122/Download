void DestroySharedImage(Display* display,
                        XImage* image,
                        XShmSegmentInfo* shminfo) {
  XShmDetach(display, shminfo);
  XDestroyImage(image);
  shmdt(shminfo->shmaddr);
}
