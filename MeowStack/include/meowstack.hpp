#ifndef MEOW_STACK_H
#define MEOW_STACK_H
#include <utility>
#include <stdexcept>

namespace MS {

template<class CAT>
class MeowStack;

template<class CAT>
struct Node {
  CAT val;
  Node* next;
};

template<class CAT>
class MeowStack {
private:
  Node<CAT>* head;
public:
  MeowStack ();

  MeowStack (const MeowStack& other);

  MeowStack (MeowStack&& other);

  ~MeowStack();

  MeowStack& operator=(const MeowStack& other);

  MeowStack& operator=(MeowStack&& other);

  void push(const CAT& value);

  void push(CAT&& value);

  void pop();

  CAT& top();
  
  const CAT& top() const;

  bool empty() const;
};


template<class CAT>
MeowStack<CAT>::MeowStack () {
  head = nullptr;
}

template<class CAT>
MeowStack<CAT>::MeowStack (const MeowStack& other) {
  if (other.head == nullptr) {
    head = nullptr;
  } else {
    head = new Node<CAT>;
    head->val = other.head->val;

    Node<CAT> *curr = head;
    Node<CAT> *curr_other = other.head->next;

    while (curr_other != nullptr) {
      curr->next = new Node<CAT>;
      curr = curr->next;
      curr->val = curr_other->val;
      curr_other = curr_other->next;
    }
    curr->next = nullptr;
  }
}

template<class CAT>
MeowStack<CAT>::MeowStack (MeowStack&& other) {
  head = other.head;
  other.head = nullptr;
}

template<class CAT>
MeowStack<CAT>::~MeowStack () {
  Node<CAT> *tmp;
  while (head != nullptr) {
    tmp = head->next;
    delete head;
    head = tmp;
  }
}

template<class CAT>
MeowStack<CAT>& MeowStack<CAT>::operator=(const MeowStack& other) {
  return *this = MeowStack(other);
}

template<class CAT>
MeowStack<CAT>& MeowStack<CAT>::operator=(MeowStack&& other) {
  std::swap(head, other.head);
  return *this;
}

template<class CAT>
void MeowStack<CAT>::push(const CAT& value) {
  Node<CAT> *newHead = new Node<CAT>;
  newHead->val = value;
  newHead->next = head;
  head = newHead;
}

template<class CAT>
void MeowStack<CAT>::push(CAT&& value) {
  push((const CAT&)value);
  // std::fstream f;
  // f.open("/home/kazenomi/MeowProgramming/meowcpu/MeowStack/tmp.txt", std::ios::app);
  // f << "push RV" << std::endl;
  // f.close();
  // Node<CAT> *newHead = new Node<CAT>;
  // newHead->val = value;
  // newHead->next = head;
  // head = newHead;
}

template<class CAT>
void MeowStack<CAT>::pop() {
  if (head == nullptr) {
    throw std::logic_error("Pop from empty stack!");
  }
  Node<CAT> *tmp = head;
  head = head->next;
  delete tmp;
}

template<class CAT>
CAT& MeowStack<CAT>::top() {
  if (head == nullptr) {
    throw std::logic_error("Top from empty stack!");
  }

  return head->val;
}

template<class CAT>
const CAT& MeowStack<CAT>::top() const {
  if (head == nullptr) {
    throw std::logic_error("Top from empty stack!");
  }

  return head->val;
}

template<class CAT>
bool MeowStack<CAT>::empty() const {
  return head == nullptr;
}

}

#endif // MEOW_STACK_H
