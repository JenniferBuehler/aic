// Echo skill: returns the input string with a server-side timestamp appended.
// Minimal pure-compute skill (no ROS clients) used to validate the skill
// author -> build -> bundle -> install -> run loop in a Flowstate process.

#ifndef ECHO_SKILL_H_
#define ECHO_SKILL_H_

#include <memory>

#include "absl/status/statusor.h"
#include "intrinsic/skills/cc/skill_interface.h"
#include "intrinsic/skills/proto/skill_service.pb.h"

class EchoSkill final : public intrinsic::skills::SkillInterface {
 public:
  // Factory used by the generated skill service main.
  static std::unique_ptr<intrinsic::skills::SkillInterface> CreateSkill();

  // Only Execute() is required: Preview()/GetFootprint() have default
  // implementations in SkillInterface.
  absl::StatusOr<std::unique_ptr<google::protobuf::Message>> Execute(
      const intrinsic::skills::ExecuteRequest& request,
      intrinsic::skills::ExecuteContext& context) override;
};

#endif  // ECHO_SKILL_H_
