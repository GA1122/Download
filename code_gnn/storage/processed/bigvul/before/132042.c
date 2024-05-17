bool LayoutBlockFlow::avoidsFloats() const
{
    return LayoutBox::avoidsFloats() || !style()->hasAutoColumnCount() || !style()->hasAutoColumnWidth();
}
