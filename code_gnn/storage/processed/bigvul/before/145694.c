String ImageInputType::resultForDialogSubmit() const
{
    StringBuilder result;
    result.appendNumber(m_clickLocation.x());
    result.append(',');
    result.appendNumber(m_clickLocation.y());
    return result.toString();
}
