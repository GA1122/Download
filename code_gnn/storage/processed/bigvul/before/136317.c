  void OnUpdateClip(const ObjectPaintProperties::UpdateResult& result,
                    bool only_updated_hit_test_values = false) {
    OnUpdate(result);
    full_context_.clip_changed |=
        !(result.Unchanged() || only_updated_hit_test_values);
  }
