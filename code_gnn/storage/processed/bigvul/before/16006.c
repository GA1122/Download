int xmp_datetime_compare(XmpDateTime *left, XmpDateTime *right)
{
    if (!left && !right) {
        return 0;
    }
    if (!left) {
        return -1;
    }
    if (!right) {
        return 1;
    }
    XMP_DateTime _left;
    ASSIGN(_left, *left);
    XMP_DateTime _right;
    ASSIGN(_right, *right);
    return XMPUtils::CompareDateTime(_left, _right);
}
