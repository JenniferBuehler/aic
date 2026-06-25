// Pure, deterministic core of the echo skill, split out so it can be
// unit-tested offline (see echo_skill_test.cc) without the cloud or the
// SDK skill-test framework.

#ifndef ECHO_LOGIC_H_
#define ECHO_LOGIC_H_

#include <string>

#include "absl/strings/str_cat.h"
#include "absl/strings/string_view.h"
#include "absl/time/time.h"

namespace b_robotized::echo_skill {

// Returns `<message> | echoed @ <RFC3339 UTC timestamp of `now`>`.
// `now` is a parameter (not absl::Now()) so the output is deterministic in
// tests.
inline std::string BuildEchoedMessage(absl::string_view message,
                                      absl::Time now) {
  return absl::StrCat(
      message, " | echoed @ ",
      absl::FormatTime(absl::RFC3339_full, now, absl::UTCTimeZone()));
}

}  // namespace b_robotized::echo_skill

#endif  // ECHO_LOGIC_H_
