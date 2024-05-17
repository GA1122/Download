static IntPoint globalPointForClientPoint(GdkWindow* window, const IntPoint& clientPoint)
{
    int x, y;
    gdk_window_get_origin(window, &x, &y);
    return clientPoint + IntSize(x, y);
}
