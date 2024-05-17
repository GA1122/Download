bool HTMLSelectElement::anonymousIndexedSetter(unsigned index, PassRefPtr<HTMLOptionElement> value, ExceptionCode& ec)
{
    if (!value) {
        ec = TypeMismatchError;
        return false;
    }
    setOption(index, value.get(), ec);
     return true;
 }
