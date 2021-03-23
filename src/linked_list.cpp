#include "linked_list.hpp"

#include <cassert>    // assert
#include <stdexcept>  // out_of_range

#include "private/internal.hpp"  // это не тот приват, о котором вы могли подумать

namespace itis {

void LinkedList::Add(Element e) {
  auto node = new Node(e, nullptr);
  if (!head_) {
    head_ = node;
    tail_ = node;
    size_ = 1;
  }
  else {
    tail_ -> next = node;
    tail_ = node;
    size_++;
  }
}

void LinkedList::Insert(int index, Element e) {
  internal::check_out_of_range(index, 0, size_ + 1);

  if (!head_) {
    auto node = new Node(e, nullptr);
    head_ = node;
    tail_ = node;
    size_ = 1;
  } else if (index == 0) {
    auto node = new Node(e, head_);
    head_ = node;
    size_++;
  } else if (index == size_) {
    auto node = new Node(e, nullptr);
    tail_ -> next = node;
    tail_ = node;
    size_++;
  } else {
      auto curr = head_;
      for (int i = 0; i < index-1; i++) {
        curr = curr -> next;
      }

      auto node = new Node(e, curr -> next);
      curr -> next = node;
      size_++;
  }
}

void LinkedList::Set(int index, Element e) {
  internal::check_out_of_range(index, 0, size_);
  auto node = find_node(index);
  node -> data = e;
}

Element LinkedList::Remove(int index) {
  internal::check_out_of_range(index, 0, size_);
  if (index == 0) {
      auto res = head_ -> data;
      auto temp = head_ -> next;
      delete head_;
      head_ = temp;
      size_--;
      return res;
  } else {
      auto node = find_node(index - 1);
      auto temp = node -> next -> next;
      auto temp_data = node -> next -> data;
      delete node -> next;
      node -> next = temp;
      size_--;
      return temp_data;
  }
}

void LinkedList::Clear() {
  auto curr = head_;
    for (int i = 0; i < size_; ++i) {
      auto temp = curr -> next;
      delete curr;
      curr = temp;
    }
    delete curr;
    size_ = 0;
    head_ = nullptr;
    tail_ = nullptr;
}

Element LinkedList::Get(int index) const {
  internal::check_out_of_range(index, 0, size_);
  auto node = find_node(index);
  return node -> data;
}

int LinkedList::IndexOf(Element e) const {
  auto curr = head_;
    for (int i = 0; i < size_; ++i) {
      if (curr -> data == e) return i;
      curr = curr -> next;
    }
  return kNotFoundElementIndex;
}

Node *LinkedList::find_node(int index) const {
  assert(index >= 0 && index < size_);
  if (index == 0) {
    return head_;
  } else if (index == size_ - 1) {
    return tail_;
  } else {
    auto curr = head_;
    for (int i = 0; i < index; ++i) {
      curr = curr -> next;
    }
    return curr;
  }
}

// РЕАЛИЗОВАНО

LinkedList::~LinkedList() {
  Clear();
}

bool LinkedList::Contains(Element e) const {
  // если индекс не найден, значит и элемента нет
  return IndexOf(e) == kNotFoundElementIndex;
}

int LinkedList::GetSize() const {
  return size_;
}

bool LinkedList::IsEmpty() const {
  return size_ == 0;
}

Element LinkedList::tail() const {
  // вместо выброса ошибки в случае nullptr, римским парламентов было решено возвращать "специальное" значение
  return tail_ ? tail_->data : Element::UNINITIALIZED;
}

Element LinkedList::head() const {
  return head_ ? head_->data : Element::UNINITIALIZED;
}

// === RESTRICTED AREA: необходимо для тестирования ===

LinkedList::LinkedList(const std::vector<Element> &elements) {
  assert(!elements.empty());

  size_ = elements.size();
  head_ = new Node(elements[0], nullptr);

  auto current_node = head_;

  for (int index = 1; index < static_cast<int>(elements.size()); index++) {
    current_node->next = new Node(elements[index], nullptr);
    current_node = current_node->next;
  }
  tail_ = current_node;
}

std::ostream &operator<<(std::ostream &os, const LinkedList &list) {
  if (list.head_ != nullptr && list.tail_ != nullptr) {
    os << "{ ";
    for (auto current_node = list.head_; current_node != list.tail_; current_node = current_node->next) {
      os << internal::elem_to_str(current_node->data) << ", ";
    }
    os << internal::elem_to_str(list.tail_->data) << " }";
  } else {
    os << "{ nullptr }";
  }
  return os;
}

bool operator==(const LinkedList &list, const std::vector<Element> &elements) {
  if (list.size_ != static_cast<int>(elements.size())) return false;
  Node *current_node = list.head_;

  for (const auto e : elements) {
    if (current_node == nullptr) return false;
    if (current_node->data != e) return false;
    current_node = current_node->next;
  }
  return true;
}

}  // namespace itis
