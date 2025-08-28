#include <iostream>
#include <cassert>

template<typename Type>
class Singleton {
  public:
    template<typename... TypeArgs>
    Singleton(TypeArgs&&... typeArgs) {
      static_assert(sizeof(storage_) >= sizeof(Type),
          "Singleton hasn't enough storage for Type");
      static_assert(std::is_standard_layout_v<Singleton<Type>>);
      static_assert(offsetof(Singleton<Type>, storage_) % alignof(Type) == 0,
          "storage_ does not meet the Type's alignment needs");
      static_assert(alignof(Singleton<Type>) % alignof(Type) == 0,
          "storage_ does not meet the Type's alignment needs");
      new (storage_) Type(std::forward<TypeArgs>(typeArgs)...);
    }
    ~Singleton() = default;

    Singleton(const Type&) = delete;
    Singleton& operator=(const Type&) = delete;

    Type* get() { return reinterpret_cast<Type*>(storage_); }

  private:
    alignas(Type) char storage_[sizeof(Type)];
};

class Foo {
  public:
    Foo(const std::string& key, uint32_t value) : key_(key), value_(value) {}
    ~Foo() = default;

    void Display() const {
      std::cout << "key:" << key_ << " value:" << value_ << std::endl;
    }

  private:
    std::string key_;
    uint32_t value_;
};

Foo* GetDefaultFoo() {
  static auto foo = Singleton<Foo>("DefaultFoo", 9527);
  return foo.get();
}

void TestSingleton() {
  auto* foo = GetDefaultFoo();
  foo->Display();
}

int main() {
  TestSingleton();
  return 0;
}
