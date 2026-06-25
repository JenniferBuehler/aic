// Offline unit test of the echo skill's core logic.
//
// NOTE: the SDK's C++ `SkillTestFactory` (intrinsic/skills/testing/
// skill_test_utils.h) is NOT linkable in this colcon/pixi build — the
// intrinsic_sdk_cmake install ships only the header, not the compiled
// implementation. So instead of exercising Execute() via the SDK harness, we
// test the extracted pure logic (echo_logic.h) directly with gtest. No cloud,
// no simulation, no SDK test framework required.

#include "echo_logic.h"

#include <string>

#include <gtest/gtest.h>

#include "absl/strings/match.h"
#include "absl/time/clock.h"
#include "absl/time/time.h"

namespace b_robotized::echo_skill {
namespace {

TEST(EchoLogicTest, PreservesMessageAndAppendsMarker) {
  const std::string out = BuildEchoedMessage("hello aic", absl::Now());
  EXPECT_TRUE(absl::StartsWith(out, "hello aic | echoed @ ")) << "got: " << out;
}

TEST(EchoLogicTest, FormatsTheGivenTimestamp) {
  // Deterministic: fixed epoch -> known RFC3339 prefix (sub-second/offset
  // suffix intentionally not asserted).
  const std::string out = BuildEchoedMessage("x", absl::FromUnixSeconds(0));
  EXPECT_TRUE(absl::StartsWith(out, "x | echoed @ 1970-01-01T00:00:00"))
      << "got: " << out;
}

}  // namespace
}  // namespace b_robotized::echo_skill
