PassRefPtr<DocumentFragment> Range::extractContents(ExceptionCode& ec)
{
    checkDeleteExtract(ec);
    if (ec)
        return 0;

    return processContents(EXTRACT_CONTENTS, ec);
}
