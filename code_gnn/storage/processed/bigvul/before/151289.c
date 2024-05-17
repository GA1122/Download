static void CreateQuad(TracedValue* value,
                       const char* name,
                       const FloatQuad& quad) {
  value->BeginArray(name);
  value->PushDouble(quad.P1().X());
  value->PushDouble(quad.P1().Y());
  value->PushDouble(quad.P2().X());
  value->PushDouble(quad.P2().Y());
  value->PushDouble(quad.P3().X());
  value->PushDouble(quad.P3().Y());
  value->PushDouble(quad.P4().X());
  value->PushDouble(quad.P4().Y());
  value->EndArray();
}
