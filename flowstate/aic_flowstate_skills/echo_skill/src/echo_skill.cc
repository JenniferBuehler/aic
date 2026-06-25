#include "echo_skill.h"

#include <memory>
#include <string>

#include "absl/log/log.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"
#include "absl/time/clock.h"
#include "absl/time/time.h"
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

  // Server-side timestamp proves the skill actually ran on the cloud side
  // (it can't be faked by the caller).
  const std::string timestamp =
      absl::FormatTime(absl::RFC3339_full, absl::Now(), absl::UTCTimeZone());
  const std::string echoed =
      absl::StrCat(params.message(), " | echoed @ ", timestamp);

  LOG(INFO) << "EchoSkill executed; echoed_message=\"" << echoed << "\"";

  auto result = std::make_unique<EchoSkillResult>();
  result->set_echoed_message(echoed);
  return result;
}
