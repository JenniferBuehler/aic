#include "echo_skill.h"

#include <memory>
#include <string>

#include "absl/log/log.h"
#include "absl/status/statusor.h"
#include "absl/time/clock.h"
#include "echo_logic.h"
#include "echo_skill.pb.h"
#include "intrinsic/skills/cc/skill_utils.h"
#include "intrinsic/skills/proto/skill_service.pb.h"
#include "intrinsic/util/status/status_macros.h"

using ::b_robotized::flowstate::EchoSkillParams;
using ::b_robotized::flowstate::EchoSkillResult;
using ::intrinsic::skills::ExecuteContext;
using ::intrinsic::skills::ExecuteRequest;
using ::intrinsic::skills::SkillInterface;

std::unique_ptr<SkillInterface> EchoSkill::CreateSkill() {
  return std::make_unique<EchoSkill>();
}

absl::StatusOr<std::unique_ptr<google::protobuf::Message>> EchoSkill::Execute(
    const ExecuteRequest& request, ExecuteContext& /*context*/) {
  INTR_ASSIGN_OR_RETURN(auto params, request.params<EchoSkillParams>());

  // Core logic lives in echo_logic.h so it can be unit-tested offline.
  // The server-side timestamp proves the skill actually ran in the cloud (the
  // caller can't fake it).
  const std::string echoed = b_robotized::echo_skill::BuildEchoedMessage(
      params.message(), absl::Now());

  LOG(INFO) << "EchoSkill executed; echoed_message=\"" << echoed << "\"";

  auto result = std::make_unique<EchoSkillResult>();
  result->set_echoed_message(echoed);
  return result;
}
