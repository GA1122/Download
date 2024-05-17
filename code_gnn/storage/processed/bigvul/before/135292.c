String Document::lastModified() const
{
    DateComponents date;
    bool foundDate = false;
    if (m_frame) {
        if (DocumentLoader* documentLoader = loader()) {
            const AtomicString& httpLastModified = documentLoader->response().httpHeaderField(HTTPNames::Last_Modified);
            if (!httpLastModified.isEmpty()) {
                date.setMillisecondsSinceEpochForDateTime(convertToLocalTime(parseDate(httpLastModified)));
                foundDate = true;
            }
        }
    }
    if (!foundDate)
        date.setMillisecondsSinceEpochForDateTime(convertToLocalTime(currentTimeMS()));
    return String::format("%02d/%02d/%04d %02d:%02d:%02d", date.month() + 1, date.monthDay(), date.fullYear(), date.hour(), date.minute(), date.second());
}
