void ImageInputType::appendToFormData(FormData& formData) const
{
    if (!element().isActivatedSubmit())
        return;
    const AtomicString& name = element().name();
    if (name.isEmpty()) {
        formData.append("x", m_clickLocation.x());
        formData.append("y", m_clickLocation.y());
        return;
    }

    DEFINE_STATIC_LOCAL(String, dotXString, (".x"));
    DEFINE_STATIC_LOCAL(String, dotYString, (".y"));
    formData.append(name + dotXString, m_clickLocation.x());
    formData.append(name + dotYString, m_clickLocation.y());

    if (!element().value().isEmpty())
        formData.append(name, element().value());
}
