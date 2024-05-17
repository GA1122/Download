static inline bool propertyTypesAreConsistent(AnimatedPropertyType expectedPropertyType, const SVGElementAnimatedPropertyList& animatedTypes)
{
    SVGElementAnimatedPropertyList::const_iterator end = animatedTypes.end();
    for (SVGElementAnimatedPropertyList::const_iterator it = animatedTypes.begin(); it != end; ++it) {
        for (size_t i = 0; i < it->properties.size(); ++i) {
            if (expectedPropertyType != it->properties[i]->animatedPropertyType()) {
                if (expectedPropertyType == AnimatedAngle && it->properties[i]->animatedPropertyType() == AnimatedEnumeration)
                    return true;
                return false;
            }
        }
    }

    return true;
}
