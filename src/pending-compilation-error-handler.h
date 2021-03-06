// Copyright 2015 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_PENDING_COMPILATION_ERROR_HANDLER_H_
#define V8_PENDING_COMPILATION_ERROR_HANDLER_H_

#include "src/base/macros.h"
#include "src/globals.h"

namespace v8 {
namespace internal {

class AstRawString;
template <class T>
class Handle;
class Isolate;
class Script;

// Helper class for handling pending compilation errors consistently in various
// compilation phases.
class PendingCompilationErrorHandler {
 public:
  PendingCompilationErrorHandler()
      : has_pending_error_(false),
        start_position_(-1),
        end_position_(-1),
        message_(nullptr),
        arg_(nullptr),
        char_arg_(nullptr),
        error_type_(kSyntaxError) {}

  void ReportMessageAt(int start_position, int end_position,
                       const char* message, const char* arg = nullptr,
                       ParseErrorType error_type = kSyntaxError) {
    if (has_pending_error_) return;
    has_pending_error_ = true;
    start_position_ = start_position;
    end_position_ = end_position;
    message_ = message;
    char_arg_ = arg;
    arg_ = nullptr;
    error_type_ = error_type;
  }

  void ReportMessageAt(int start_position, int end_position,
                       const char* message, const AstRawString* arg,
                       ParseErrorType error_type = kSyntaxError) {
    if (has_pending_error_) return;
    has_pending_error_ = true;
    start_position_ = start_position;
    end_position_ = end_position;
    message_ = message;
    char_arg_ = nullptr;
    arg_ = arg;
    error_type_ = error_type;
  }

  bool has_pending_error() const { return has_pending_error_; }

  void ThrowPendingError(Isolate* isolate, Handle<Script> script);

 private:
  bool has_pending_error_;
  int start_position_;
  int end_position_;
  const char* message_;
  const AstRawString* arg_;
  const char* char_arg_;
  ParseErrorType error_type_;

  DISALLOW_COPY_AND_ASSIGN(PendingCompilationErrorHandler);
};

}  // namespace internal
}  // namespace v8
#endif  // V8_PENDING_COMPILATION_ERROR_HANDLER_H_
