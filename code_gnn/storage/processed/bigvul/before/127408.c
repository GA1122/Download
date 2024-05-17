static StylePropertySet* rightToLeftDeclaration()
{
    DEFINE_STATIC_REF(MutableStylePropertySet, rightToLeftDecl, (MutableStylePropertySet::create()));
    if (rightToLeftDecl->isEmpty())
        rightToLeftDecl->setProperty(CSSPropertyDirection, CSSValueRtl);
    return rightToLeftDecl;
}
