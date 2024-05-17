void Scrollbar::paint(GraphicsContext* context, const IntRect& damageRect)
{
    if (context->updatingControlTints() && theme()->supportsControlTints()) {
        invalidate();
        return;
    }

    if (context->paintingDisabled() || !frameRect().intersects(damageRect))
        return;

    if (!theme()->paint(this, context, damageRect))
        Widget::paint(context, damageRect);
}
