#pragma once
#include <memory>
#include <vector>

class ValidationNode {
public:
  ValidationNode() = default;
  ValidationNode(const ValidationNode& other) = delete;
  ~ValidationNode();

  bool is_valid() const;
  void invalidate();

  void clear();
  void add_dependent(ValidationNode& other);

private:
  std::vector<ValidationNode*> parents_;
  std::vector<ValidationNode*> children_;

  bool is_valid_ = false;
};
