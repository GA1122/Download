static bool isValidConstraint(const WebString& constraint)
{
    return isSupportedConstraint(constraint) || constraint == "valid_but_unsupported_1" || constraint == "valid_but_unsupported_2";
}
