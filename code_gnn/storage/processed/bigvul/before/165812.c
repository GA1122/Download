static inline void NotifyAnimValChanged(SVGElement* target_element,
                                        const QualifiedName& attribute_name) {
  target_element->InvalidateSVGAttributes();
  target_element->SvgAttributeChanged(attribute_name);
}
