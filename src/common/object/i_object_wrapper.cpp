#include "i_object_wrapper.hpp"

template <class TWrapper>
std::unique_ptr<TWrapper>& IObjectWrapper::get_or_create_wrapper_for(
    struct Object* wrapped_object) {
  auto &wrapper = wrappers[wrapped_object];
  if (wrapper != nullptr) {
    return wrapper;
  }

  return wrappers[wrapped_object] = std::make_unique<TWrapper>(wrapper);
}

std::unique_ptr<IObjectWrapper>& IObjectWrapper::get_wrapper_for(struct Object* wrapped_object) {
  return wrappers[wrapped_object];
}

// TODO: Don't make this such an easily accessed public method...
void IObjectWrapper::destroy_wrapper_for(struct Object* wrapped_object) {
  wrappers.erase(wrapped_object);
}

IObjectWrapper::IObjectWrapper(struct Object* wrapped_object) {
  this->wrapped_object = wrapped_object;

  wrappers[wrapped_object] = std::unique_ptr<IObjectWrapper>(this);
}

std::map<struct Object*, std::unique_ptr<IObjectWrapper>> IObjectWrapper::
wrappers;
