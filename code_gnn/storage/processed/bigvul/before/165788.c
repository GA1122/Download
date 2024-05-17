static void ForSelfAndInstances(SVGElement* element, T callback) {
  SVGElement::InstanceUpdateBlocker blocker(element);
  callback(element);
  for (SVGElement* instance : element->InstancesForElement())
    callback(instance);
}
