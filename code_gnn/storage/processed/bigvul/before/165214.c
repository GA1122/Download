  static ash::mojom::VoiceInteractionState Convert(
      arc::mojom::VoiceInteractionState state) {
    switch (state) {
      case arc::mojom::VoiceInteractionState::NOT_READY:
        return ash::mojom::VoiceInteractionState::NOT_READY;
      case arc::mojom::VoiceInteractionState::STOPPED:
        return ash::mojom::VoiceInteractionState::STOPPED;
      case arc::mojom::VoiceInteractionState::RUNNING:
        return ash::mojom::VoiceInteractionState::RUNNING;
    }

    NOTREACHED() << "Invalid state: " << static_cast<int>(state);
    return ash::mojom::VoiceInteractionState::NOT_READY;
  }
