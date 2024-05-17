LayoutBlockFlow::FlowThreadType LayoutBlockFlow::flowThreadType(const ComputedStyle& style)
{
    if (isPagedOverflow(style))
        return PagedFlowThread;
    if (style.specifiesColumns())
        return MultiColumnFlowThread;
    return NoFlowThread;
}
