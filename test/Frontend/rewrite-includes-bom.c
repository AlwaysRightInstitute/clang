// RUN: grep -E "^\\xEF\\xBB\\xBF" %S/Inputs/rewrite-includes-bom.h
// RUN: %clang_cc1 -E -frewrite-includes -I %S/Inputs %s -o - | %clang_cc1 -fsyntax-only -verify -x c - | not grep -E "\\xEF\\xBB\\xBF"
// expected-no-diagnostics

#include "rewrite-includes-bom.h"