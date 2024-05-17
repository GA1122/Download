bool RenderBox::autoscrollInProgress() const
{
    return frame() && frame()->page() && frame()->page()->autoscrollController().autoscrollInProgress(this);
}
