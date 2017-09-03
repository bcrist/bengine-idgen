#include "size_of_type.hpp"

#include <be/core/namespaced_id.hpp>
#include <be/util/chunked_list.hpp>
#include <be/core/lifecycle.hpp>
#include <be/core/filesystem.hpp>
#include <be/core/time.hpp>
#include <be/core/extents.hpp>
#include <be/gfx/tex/texture.hpp>

#include <array>
#include <vector>
#include <deque>
#include <memory>
#include <list>
#include <forward_list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <functional>
#include <thread>
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <ctime>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <boost/circular_buffer.hpp>
#include <boost/container/vector.hpp>
#include <boost/container/small_vector.hpp>
#include <boost/container/stable_vector.hpp>
#include <boost/container/static_vector.hpp>
#include <boost/container/deque.hpp>
#include <boost/container/list.hpp>
#include <boost/container/slist.hpp>
#include <boost/container/map.hpp>
#include <boost/container/flat_map.hpp>
#include <boost/container/set.hpp>
#include <boost/container/flat_set.hpp>
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>

#include <boost/function.hpp>

using namespace std;
using namespace glm;

namespace be {

class TestClass {
public:
   int x;
   void f() {
      cout << "T::f!";
   }
};

#define sz(...) size_of_type.size< __VA_ARGS__ >(#__VA_ARGS__)
#define szn(...) size_of_type.size< __VA_ARGS__ >(#__VA_ARGS__, LogColor::yellow, true)
#define bsz(...) size_of_type.size< __VA_ARGS__ >(#__VA_ARGS__, LogColor::bright_yellow)
#define bszn(...) size_of_type.size< __VA_ARGS__ >(#__VA_ARGS__, LogColor::bright_yellow, true)
#define br size_of_type.separator(' ')
#define hr size_of_type.separator('-')
#define section (br, size_of_type.separator('='), br)

void sizeof_main(SizeOfType& size_of_type) {
   using T = TestClass;

   szn(short); szn(int); szn(long); szn(long long); br;
   szn(float); szn(double); szn(long double); br;
   szn(wchar_t); szn(char16_t); szn(char32_t);
   hr;
   szn(uint8_t); szn(int8_t); szn(int_least8_t); szn(int_fast8_t); br;
   szn(uint16_t); szn(int16_t); szn(int_least16_t); szn(int_fast16_t); br;
   szn(uint32_t); szn(int32_t); szn(int_least32_t); szn(int_fast32_t); br;
   szn(uint64_t); szn(int64_t); szn(int_least64_t); szn(int_fast64_t);
   hr;
   bszn(U8); bszn(I8); bszn(U16); bszn(I16); bszn(U32); bszn(I32); bszn(U64); bszn(I64); br;
   bszn(F32); bszn(F64); br;
   bszn(UC); bszn(SC); br;
   bszn(UI); bszn(SI);
   hr;
   szn(size_t); szn(max_align_t); szn(ptrdiff_t); br;
   szn(int*); szn(nullptr_t); szn(intptr_t); szn(time_t);
   hr;
   sz(int T::*);
   sz(void(*)());
   sz(void(T::*)());
   sz(function<void()>);
   sz(boost::function<void()>);
   hr;
   sz(exception); sz(exception_ptr);
   br;
   sz(string); sz(wstring);
   br;
   sz(unique_ptr<int>); sz(unique_ptr<int[]>); sz(shared_ptr<int>); sz(weak_ptr<int>);

   section;

   sz(vec2); sz(vec3); sz(vec4); sz(quat); br;
   sz(dvec2); sz(dvec3); sz(dvec4); sz(dquat); br;
   sz(ivec2); sz(ivec3); sz(ivec4); br;
   sz(bvec2); sz(bvec3); sz(bvec4); br;
   sz(mat2); sz(mat2x3); sz(mat2x4); sz(mat3); sz(mat3x4); sz(mat4); br;
   sz(dmat2); sz(dmat2x3); sz(dmat2x4); sz(dmat3); sz(dmat3x4); sz(dmat4);

   section;

   sz(array<char, 0>);
   sz(array<int, 0>);
   sz(array<intptr_t, 0>);
   sz(boost::container::static_vector<char, 0>);
   sz(boost::container::static_vector<intptr_t, 0>);
   sz(boost::container::static_vector<int, 0>);
   sz(boost::container::small_vector<char, 0>);
   sz(boost::container::small_vector<int, 0>);
   sz(boost::container::small_vector<intptr_t, 0>);
   br;

   sz(array<char, 1>);
   sz(array<int, 1>);
   sz(array<intptr_t, 1>);
   sz(boost::container::static_vector<char, 1>);
   sz(boost::container::static_vector<intptr_t, 1>);
   sz(boost::container::static_vector<int, 1>);
   sz(boost::container::small_vector<char, 1>);
   sz(boost::container::small_vector<int, 1>);
   sz(boost::container::small_vector<intptr_t, 1>);
   br;

   sz(array<char, 100>);
   sz(array<int, 100>);
   sz(array<intptr_t, 100>);
   sz(boost::container::static_vector<char, 100>);
   sz(boost::container::static_vector<intptr_t, 100>);
   sz(boost::container::static_vector<int, 100>);
   sz(boost::container::small_vector<char, 100>);
   sz(boost::container::small_vector<int, 100>);
   sz(boost::container::small_vector<intptr_t, 100>);
   br;

   sz(vector<int>);
   sz(boost::container::vector<int>);
   sz(boost::container::stable_vector<int>);
   sz(priority_queue<int>);
   br;

   sz(deque<int>);
   sz(boost::container::deque<int>);
   sz(boost::circular_buffer<int>);
   sz(boost::circular_buffer_space_optimized<int>);
   sz(stack<int>);
   sz(queue<int>);
   br;

   sz(list<int>); sz(boost::container::list<int>); br;
   sz(forward_list<int>); sz(boost::container::slist<int>);
   bsz(util::ChunkedList<int, std::allocator<int>, 16, 7, 1>);
   bsz(util::ChunkedList<int, std::allocator<int>, 16, 7, 2>);
   bsz(util::ChunkedList<int, std::allocator<int>, 16, 7, 6>);

   hr;

   sz(set<int>); sz(boost::container::set<int>); sz(boost::container::flat_set<int>); br;
   sz(multiset<int>); sz(boost::container::multiset<int>); sz(boost::container::flat_multiset<int>); br;

   sz(map<int, int>); sz(boost::container::map<int, int>); sz(boost::container::flat_map<int, int>); br;
   sz(multimap<int, int>); sz(boost::container::multimap<int, int>); sz(boost::container::flat_multimap<int, int>); br;

   sz(unordered_set<int>); sz(boost::unordered_set<int>); br;
   sz(unordered_multiset<int>); sz(boost::unordered_multiset<int>); br;

   sz(unordered_map<int, int>); sz(boost::unordered_map<int, int>); br;
   sz(unordered_multimap<int, int>); sz(boost::unordered_multimap<int, int>);

   section;

   bszn(TU);
   bsz(S);
   szn(Path);
   br;

   bsz(Id);
   bsz(NamespacedId);
   br;

   //NL;
   bszn(be::range);
   bszn(be::drange);
   bszn(be::irange);
   bszn(be::rect);
   bszn(be::drect);
   bszn(be::irect);
   bszn(be::box);
   bszn(be::dbox);
   bszn(be::ibox);
   br;

   bsz(be::gfx::tex::TextureStorage);
   bsz(be::gfx::tex::ImageFormat);
   bsz(be::gfx::tex::TextureView);
   bsz(be::gfx::tex::ImageView);
};

} // be

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
   be::CoreInitLifecycle init;

   be::SizeOfType size_of_type;
   if (argc > 1) {
      size_of_type.preview_chars = std::max(0ll, strtoll(argv[1], nullptr, 0));
   }
   if (argc > 2) {
      size_of_type.bits_per_preview_char = std::max(1ll, strtoll(argv[2], nullptr, 0));
   }
   if (argc > 3) {
      size_of_type.preview_char_grouping = std::max(0ll, strtoll(argv[3], nullptr, 0));
   }
   be::sizeof_main(size_of_type);
   std::cout << be::color::gray;
   return 0;
}
