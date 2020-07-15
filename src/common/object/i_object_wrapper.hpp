#pragma once

#include <map>
#include <memory>

/**
 * Interface for "object wrappers", which allows us to reframe Super Mario 64's
 * C-style instance system in an C++ way.
 */
class IObjectWrapper {
public:
  static std::unique_ptr<IObjectWrapper>& get_wrapper_for(
    struct Object* wrapped_object);
  // TODO: Don't make this such an easily accessed public method...
  static void destroy_wrapper_for(struct Object* wrapped_object);

  IObjectWrapper(struct Object* wrapped_object);

protected:
  struct Object* wrapped_object;

private:
  static std::map<struct Object*, std::unique_ptr<IObjectWrapper>> wrappers;
};
