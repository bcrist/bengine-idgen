#include "idgen_app.hpp"

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
   be::idgen::IdGenApp app(argc, argv);
   return app();
}
