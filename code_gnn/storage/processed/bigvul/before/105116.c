void Range::deleteContents(ExceptionCode& ec)
{
    checkDeleteExtract(ec);
    if (ec)
        return;

    processContents(DELETE_CONTENTS, ec);
}
