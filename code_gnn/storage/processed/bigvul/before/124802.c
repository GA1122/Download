bool RenderBox::autoWidthShouldFitContent() const
{
    return node() && (isHTMLInputElement(*node()) || isHTMLSelectElement(*node()) || isHTMLButtonElement(*node())
        || isHTMLTextAreaElement(*node()) || (isHTMLLegendElement(*node()) && !style()->hasOutOfFlowPosition()));
}
