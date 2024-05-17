void HTMLInputElement::setSize(unsigned size, ExceptionCode& ec)
{
    if (!size)
        ec = INDEX_SIZE_ERR;
    else
        setSize(size);
}
