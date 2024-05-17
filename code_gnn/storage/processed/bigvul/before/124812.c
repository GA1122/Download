IntSize RenderBox::calculateAutoscrollDirection(const IntPoint& windowPoint) const
{
    if (!frame())
        return IntSize();

    FrameView* frameView = frame()->view();
    if (!frameView)
        return IntSize();

    IntRect box(absoluteBoundingBoxRect());
    box.move(view()->frameView()->scrollOffset());
    IntRect windowBox = view()->frameView()->contentsToWindow(box);

    IntPoint windowAutoscrollPoint = windowPoint;

    if (windowAutoscrollPoint.x() < windowBox.x() + autoscrollBeltSize)
        windowAutoscrollPoint.move(-autoscrollBeltSize, 0);
    else if (windowAutoscrollPoint.x() > windowBox.maxX() - autoscrollBeltSize)
        windowAutoscrollPoint.move(autoscrollBeltSize, 0);

    if (windowAutoscrollPoint.y() < windowBox.y() + autoscrollBeltSize)
        windowAutoscrollPoint.move(0, -autoscrollBeltSize);
    else if (windowAutoscrollPoint.y() > windowBox.maxY() - autoscrollBeltSize)
        windowAutoscrollPoint.move(0, autoscrollBeltSize);

    return windowAutoscrollPoint - windowPoint;
}
