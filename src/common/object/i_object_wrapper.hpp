#pragma once

#include <map>
#include <memory>

/**
 * Interface for "object wrappers", which allows us to reframe Super Mario 64's
 * C-style instance system in an C++ way.
 */
// TODO: Move these static methods to a management service.
class IObjectWrapper {
public:
  template<class TWrapper>
  static std::unique_ptr<TWrapper>& get_or_create_wrapper_for(
      struct Object* wrapped_object);
  static std::unique_ptr<IObjectWrapper>& get_wrapper_for(
    struct Object* wrapped_object);
  // TODO: Don't make this such an easily accessed public method...
  static void destroy_wrapper_for(struct Object* wrapped_object);

  IObjectWrapper(struct Object* wrapped_object);

  virtual void init() {}
  virtual void tick() {}

protected:
  struct Object* wrapped_object;

private:
  static std::map<struct Object*, std::unique_ptr<IObjectWrapper>> wrappers;
};
