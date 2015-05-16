#include <iostream>
#include <string>
#include <utility>
#include <memory>
#include <algorithm>
class StrVec {
public:
  StrVec() : elements(nullptr), first_free(nullptr), cap(nullptr) {}
  StrVec(const StrVec &);
  StrVec(StrVec &&) noexcept;
  StrVec &operator=(const StrVec &);
  StrVec &operator=(StrVec &&) noexcept;
  ~StrVec() { free(); }
  void push_back(const std::string&);
  size_t size() const { return first_free - elements; }
  size_t capacity() const { return cap - elements; }
  std::string *begin() const { return elements; }
  std::string *end() const { return first_free; }
private:
  std::allocator<std::string> alloc;
  void chk_n_alloc() {
    if (size() == capacity()) reallocate();
  }
  std::pair<std::string *, std::string *> alloc_n_copy (
        const std::string *, const std::string *);
  void free();
  void reallocate();
  std::string *elements;
  std::string *first_free;
  std::string *cap;
};

void StrVec::push_back(const std::string& s) {
  chk_n_alloc();
  alloc.construct(first_free++, s);
}

std::pair<std::string *, std::string *> StrVec::alloc_n_copy(
    const std::string *b, const std::string *e) {
  auto data = alloc.allocate(e - b);
  return { data, std::uninitialized_copy(b, e, data)};
}

void StrVec::free() {
  if (elements) {
    for (auto p = first_free; p != elements; /* empty */)
      alloc.destroy(--p);

    alloc.deallocate(elements, cap - elements);
  }
}

StrVec::StrVec(const StrVec &s) {
  auto newdata = alloc_n_copy(s.begin(), s.end());
  elements = newdata.first;
  first_free = cap = newdata.second;
}

StrVec &StrVec::operator=(const StrVec &rhs) {
  auto data = alloc_n_copy(rhs.begin(), rhs.end());
  free();
  elements = data.first;
  first_free = cap = data.second;
  return *this;
}

void StrVec::reallocate() {
  auto newcapacity = size() ? 2 * size() : 1;
  auto newdata = alloc.allocate(newcapacity);
  auto dest = newdata;
  auto elem = elements;
  for (size_t i = 0; i != size(); ++i)
    alloc.construct(dest++, std::move(*elem++));

  free();
  elements = newdata;
  first_free = dest;
  cap = elements + newcapacity;
}

StrVec::StrVec(StrVec &&s) noexcept : elements(s.elements), first_free(s.first_free), cap(s.cap) {
  s.elements = s.first_free = s.cap = nullptr;
}

StrVec &StrVec::operator=(StrVec &&rhs) noexcept {
  if (this != &rhs) {
    free(); 
    elements = rhs.elements;
    first_free = rhs.first_free;
    cap = rhs.cap;
    rhs.elements = rhs.first_free = rhs.cap = nullptr;
  }
  return *this;
}

int main() {
  StrVec svec;

  svec.push_back("aaa");
  svec.push_back("bbb");
  svec.push_back("ccc");

  /**
   * test the lambda expression, comment the regular scope of the strvec
   */
  /*
  for (auto it = svec.begin(); it != svec.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << "size of svec is : "  << svec.size() << std::endl;
  */

  std::for_each(svec.begin(), svec.end(), [](std::string &s) { std::cout << s << std::endl;}); 

  return 0;
}
