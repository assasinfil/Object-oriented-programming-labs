#include <list>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <utility>

using namespace std;

bool is_fibonacci_number(int n) {
    if (n == 1) return true;
    auto number1 = fma(5.0, pow(n, 2), -4);
    auto number2 = fma(5.0, pow(n, 2), 4);
    auto test1 = sqrt(number1);
    auto test2 = sqrt(number2);
    return (test1 * test1 == number1) ^ (test2 * test2 == number2);
}


template<class T>
void push(list<T> &data, T element) {
    auto it = data.begin();
    for (const auto &item: data) {
        if (element < item) {
            data.insert(it, element);
            return;
        }
        it++;
    }
    data.insert(it, element);
}

list<int> filter(list<int> &data) {
    list<int> filtered_data(data.size());
    auto it = copy_if(data.begin(), data.end(), filtered_data.begin(), is_fibonacci_number);
    filtered_data.resize(distance(filtered_data.begin(), it));
    return filtered_data;
}

class Stadium {
private:
    string name, year_of_construction;
    int capacity{}, kinds_of_sport{}, number_of_arenas{};
public:
    Stadium() = default;

    Stadium(string name, int kindsOfSport, string yearOfConstruction, int capacity, int numberOfArenas)
            : name(std::move(name)), kinds_of_sport(kindsOfSport), year_of_construction(std::move(yearOfConstruction)),
              capacity(capacity),
              number_of_arenas(numberOfArenas) {}

    [[nodiscard]] const string &getName() const {
        return name;
    }

    [[nodiscard]] int getKindsOfSport() const {
        return kinds_of_sport;
    }

    [[nodiscard]] const string &getYearOfConstruction() const {
        return year_of_construction;
    }

    [[nodiscard]] int getCapacity() const {
        return capacity;
    }

    [[nodiscard]] int getNumberOfArenas() const {
        return number_of_arenas;
    }

    bool operator<(const Stadium &rhs) const {
        if (name < rhs.name) return true;
        if (name > rhs.name) return false;
        if (kinds_of_sport < rhs.kinds_of_sport) return true;
        if (kinds_of_sport > rhs.kinds_of_sport) return false;
        if (year_of_construction < rhs.year_of_construction) return true;
        if (year_of_construction > rhs.year_of_construction) return false;
        if (capacity < rhs.capacity) return true;
        if (capacity > rhs.capacity) return false;
        return number_of_arenas < rhs.number_of_arenas;
    }

    bool operator>(const Stadium &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const Stadium &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const Stadium &rhs) const {
        return !(*this < rhs);
    }

    virtual ~Stadium() {
        name.clear();
        year_of_construction.clear();
    }

    friend ostream &operator<<(ostream &outStream, Stadium const &obj);
};


template<class T>
T *pop(list<T> &data) {
    auto *object = &data.back();
    data.pop_back();
    return object;
}
///////////////////////



// Элемент связного списка
template<class T>
class Element {
private:
    // указатель на следующий элемент
    Element *next;
    // указатель на предыдущий элемент
    Element *prev;
    // информация, хранимая в поле
    T field;
public:
    explicit Element(const T &value, Element<T> *next_ptr = nullptr, Element<T> *prev_ptr = nullptr) : field(value) {
        next = next_ptr;
        prev = prev_ptr;
    }

    // доступ к полю *next
    virtual Element *getNext() { return next; }

    virtual void setNext(Element *value) { next = value; }

    // доступ к полю *prev
    virtual Element *getPrevious() { return prev; }

    virtual void setPrevious(Element *value) { prev = value; }

    // доступ к полю с хранимой информацией field
    virtual T getValue() { return field; }

    virtual void setValue(T value) { field = value; }

    template<class D>
    friend ostream &operator<<(ostream &outputStream, Element<T> &obj);
};

template<class T>
ostream &operator<<(ostream &outputStream, Element<T> &obj) {
    outputStream << obj.getValue();
    return outputStream;
}

template<class T>
class LinkedListParent {
protected:
    // достаточно хранить начало и конец
    Element<T> *head;
    Element<T> *tail;
    // для удобства храним количество элементов
    int num;
public:
    virtual int Number() { return num; }

    virtual Element<T> *getBegin() { return head; }

    virtual Element<T> *getEnd() { return tail; }

    virtual void setBegin(Element<T> *element) { head = element; }

    virtual void setEnd(Element<T> *element) { tail = element; }

    LinkedListParent() {
        // конструктор без параметров
        cout << "Parent constructor" << endl;
        head = nullptr;
        tail = nullptr;
        num = 0;
    }

    // чисто виртуальная функция: пока не определимся с типом списка, не сможем реализовать добавление
    virtual Element<T> *push(const T &value) = 0;

    // чисто виртуальная функция: пока не определимся с типом списка, не сможем реализовать удаление
    virtual Element<T> *pop() = 0;

    // деструктор - освобождение памяти
    virtual ~LinkedListParent() {
        cout << "Parent destructor" << endl;
    }

    // получение элемента по индексу - какова асимптотическая оценка этого действия?
    virtual Element<T> *operator[](int i) {
        // индексация
        if (i < 0 || i > num) return nullptr;
        // ищем i-й элемент - вставим в начало и отсчитываем i шагов вперед
        auto *cur = head;
        for (auto k = 0; k < i; ++k) cur = cur->getNext();
        return cur;
    }

    template<class U>
    friend ostream &operator<<(ostream &outStream, LinkedListParent<T> &obj);

    template<class U>
    friend istream &operator>>(istream &inputStream, LinkedListParent<T> &obj);
};


template<class T>
ostream &operator<<(ostream &outStream, LinkedListParent<T> &obj) {
    outStream << "Length: " << obj.Number() << endl;
    auto i = 0;
    for (auto *current = obj.getBegin(); current != nullptr; current = current->getNext()) {
        outStream << "arr[" << i << "] = " << current->getValue() << endl;
        ++i;
    }
    return outStream;
}

template<class T>
istream &operator>>(istream &inputStream, LinkedListParent<T> &obj) {
    // чтение из файла и консоли совпадают
    unsigned int len;
    inputStream >> len;
    // здесь надо очистить память под obj, установить obj.num = 0
    T v = 0;
    for (auto i = 0; i < len; ++i) {
        inputStream >> v;
        obj.push(v);
    }
    return inputStream;
}

ostream &operator<<(ostream &outStream, Stadium const &obj) {
    outStream << obj.getName() << ' ' << obj.getKindsOfSport() << ' ' << obj.getYearOfConstruction()
              << ' '
              << obj.getCapacity() << ' ' << obj.getNumberOfArenas() << endl;
    return outStream;
}

template<typename ValueType>
class ListIterator : public std::iterator<std::input_iterator_tag, ValueType> {
public:
    ListIterator() {
        ptr = nullptr;
    }

    explicit ListIterator(Element<ValueType> *p) {
        ptr = p;
    }

    ListIterator(const ListIterator &it) {
        ptr = it.ptr;
    }

    bool operator!=(ListIterator const &other) const {
        return ptr != other.ptr;
    }

    // need for BOOST_FOREACH
    bool operator==(ListIterator const &other) const {
        return ptr == other.ptr;
    }

    Element<ValueType> &operator*() {
        return *ptr;
    }

    ListIterator &operator++() {
        ptr = ptr->getNext();
        return *this;
    }

    ListIterator<ValueType> operator++(int v) {
        ptr = ptr->getNext();
        return *this;
    }

    ListIterator &operator--() {
        ptr = ptr->getPrevious();
        return *this;
    }

    ListIterator<ValueType> operator--(int v) {
        ptr = ptr->getPrevious();
        return *this;
    }

    ListIterator &operator=(const ListIterator &it) {
        if (it != this) {
            ptr = it.ptr;
        }
        return *this;
    }

    ListIterator &operator=(Element<ValueType> *p) {
        ptr = p;
        return *this;
    }

private:
    Element<ValueType> *ptr;
};

template<class T>
class IteratedLinkedList : public LinkedListParent<T> {
public:
    ListIterator<T> iterator;

    IteratedLinkedList() : LinkedListParent<T>() {
        cout << "IteratedLinkedList constructor" << endl;
    }

    ~IteratedLinkedList() override {
        cout << "IteratedLinkedList destructor" << endl;
    }

    ListIterator<T> begin() {
        ListIterator<T> it;
        it = this->head;
        return it;
    }

    ListIterator<T> end() {
        ListIterator<T> it;
        it = this->tail;
        ++it;
        return it;
    }
};

template<class T>
class DoubleLinkedList : public IteratedLinkedList<T> {
public:
    DoubleLinkedList() = default;

    ~DoubleLinkedList() override = default;

    Element<T> *push(const T &value) override {
        Element<T> *end = LinkedListParent<T>::getEnd();
        auto *element = new Element<T>(value, nullptr, end);
        if (end == nullptr) {
            LinkedListParent<T>::setBegin(element);
            LinkedListParent<T>::setEnd(element);
        } else {
            end->setNext(element);
            LinkedListParent<T>::setEnd(element);
        }
        LinkedListParent<T>::num++;
        return element;
    }

    Element<T> *pop() override {
        auto end = LinkedListParent<T>::tail;
        auto previous = end->getPrevious();
        previous->setNext(nullptr);
        end->setPrevious(nullptr);
        LinkedListParent<T>::num--;
        return end;
    }

    template<class U>
    friend ostream &operator<<(ostream &outStream, DoubleLinkedList<T> &obj);
};

template<class T>
ostream &operator<<(ostream &outStream, DoubleLinkedList<T> &obj) {
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        outStream << *it << endl;
    }
    return outStream;
}

///////////////////////

int main() {
    // Task 1
    list<int> data;
    for (auto i = 10000; i >= 0; --i) {
        push(data, i);
    }
    push(data, 10);
    cout << *pop(data) << endl;
    auto fibonacci_numbers = filter(data);
    for (auto item: fibonacci_numbers) {
        cout << item << ' ';
    }
    cout << endl;

    // Task 2
    list<Stadium> stadiums;

    push(stadiums, Stadium("b", 1, "1", 1, 2));
    push(stadiums, Stadium("a", 1, "1", 1, 1));
    push(stadiums, Stadium("a", 1, "1", 1, 2));

    for (const auto &item: stadiums) {
        cout << item.getName() << ' ' << item.getKindsOfSport() << ' ' << item.getYearOfConstruction()
             << ' '
             << item.getCapacity() << ' ' << item.getNumberOfArenas() << endl;
    }
    cout << endl;

    // Task 3
    auto stadium = pop(stadiums);
    cout << stadium->getName();

    for (const auto &item: stadiums) {
        cout << item.getName() << ' ' << item.getKindsOfSport() << ' ' << item.getYearOfConstruction()
             << ' '
             << item.getCapacity() << ' ' << item.getNumberOfArenas() << endl;
    }
    // Task 4
    DoubleLinkedList<int> test1;
    test1.push(10);
    test1.push(11);
    test1.push(12);
    cout << test1;
    cout << endl << test1.pop()->getValue() << endl;
    cout << test1;
    cout << endl;

    // Task 5
    DoubleLinkedList<Stadium> test2;
    test2.push(Stadium("b", 1, "1", 1, 2));
    test2.push(Stadium("a", 1, "1", 1, 2));
    cout << test2;
    return 0;
}
