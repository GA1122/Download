bool HTMLInputElement::tooLong(const String& value, NeedsToCheckDirtyFlag check) const
{
    if (!isTextType())
        return false;
    int max = maxLength();
    if (max < 0)
        return false;
    if (check == CheckDirtyFlag) {
        if (!hasDirtyValue() || !m_wasModifiedByUser)
            return false;
    }
    return numGraphemeClusters(value) > static_cast<unsigned>(max);
}
