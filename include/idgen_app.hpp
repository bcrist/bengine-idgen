#pragma once
#ifndef IDGEN_APP_HPP_
#define IDGEN_APP_HPP_

#include "mode.hpp"
#include "format.hpp"
#include "type.hpp"
#include <be/core/lifecycle.hpp>
#include <be/core/filesystem.hpp>

namespace be::idgen {

///////////////////////////////////////////////////////////////////////////////
class IdGenApp final {
public:
   IdGenApp(int argc, char** argv);
   int operator()();

private:
   void process_(const S& input);

   CoreInitLifecycle init_;
   //CoreLifecycle core_;

   I8 status_ = 0;

   Mode mode_ = Mode::fnv1a;
   U16 bits_ = 64;
   U64 basis_ = 0;
   bool use_basis_ = false;

   bool first_ = true;
   Format format_ = Format::standard;
   Type type_ = Type::hash;
   S separator_ = "\r\n";

   bool interactive_ = false;
};

} // be::idgen

#endif
