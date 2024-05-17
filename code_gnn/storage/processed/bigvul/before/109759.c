static void printNavigationErrorMessage(Frame* frame, const KURL& activeURL, const char* reason)
{
    String message = "Unsafe JavaScript attempt to initiate navigation for frame with URL '" + frame->document()->url().string() + "' from frame with URL '" + activeURL.string() + "'. " + reason + "\n";

    frame->domWindow()->printErrorMessage(message);
}
