 void HTMLStyleElement::RemovedFrom(ContainerNode* insertion_point) {
   HTMLElement::RemovedFrom(insertion_point);
   StyleElement::RemovedFrom(*this, insertion_point);
 }
