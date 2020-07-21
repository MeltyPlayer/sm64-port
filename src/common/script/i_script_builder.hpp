#pragma once

#include <memory>

template <typename TScript>
class IScriptPart;

template <typename TScript>
class IScriptBuilder {
public:
  IScriptBuilder() {}
  IScriptBuilder(const IScriptBuilder& other) = delete;

  virtual IScriptBuilder<TScript>& add_part(
      std::shared_ptr<IScriptPart<TScript>> part) = 0;

  virtual IScriptBuilder<TScript>& add_script(TScript script) = 0;
  virtual IScriptBuilder<TScript>& add_scripts(
      std::initializer_list<const TScript> scripts) = 0;
  virtual IScriptBuilder<TScript>& add_scripts(const TScript* scripts,
                                               int script_count) = 0;

  virtual IScriptBuilder<TScript>& add_builder(
      std::shared_ptr<IScriptBuilder<TScript>> other) = 0;

  virtual int size() const = 0;

  virtual void build_into(TScript* dst, int& dst_pos) const = 0;
};
