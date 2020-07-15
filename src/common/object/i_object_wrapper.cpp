#include "i_object_wrapper.hpp"

std::unique_ptr<IObjectWrapper>& IObjectWrapper::
get_wrapper_for(struct Object* wrapped_object) {
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
