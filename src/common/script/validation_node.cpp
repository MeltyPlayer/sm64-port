#include "validation_node.hpp"

ValidationNode::~ValidationNode() { clear(); }


bool ValidationNode::is_valid() const { return is_valid_; }

void ValidationNode::invalidate() {
  if (!is_valid_) { return; }

  for (auto& parent : parents_) {
    parent->invalidate();
  }
}


void ValidationNode::clear() {
  children_.clear();
  invalidate();
}

void ValidationNode::add_dependent(ValidationNode& other) {
  children_.push_back(&other);
  other.parents_.push_back(this);

  invalidate();
}
