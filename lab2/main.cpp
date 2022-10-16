#include <map>
#include <algorithm>
#include <iostream>
#include <queue>
#include <cmath>
#include <utility>

#define threshold 4

using namespace std;

class Stadium {
private:
    string name;
    int year;
    int capacity;
public:
    Stadium(string name, int year, int capacity) : name(std::move(name)), year(year), capacity(capacity) {}

    bool operator==(const Stadium &rhs) const {
        return name == rhs.name &&
               year == rhs.year &&
               capacity == rhs.capacity;
    }

    bool operator!=(const Stadium &rhs) const {
        return !(rhs == *this);
    }

    friend ostream &operator<<(ostream &os, const Stadium &stadium) {
        os << "name: " << stadium.name << " year: " << stadium.year << " capacity: " << stadium.capacity << endl;
        return os;
    }
};

pair<string, int> find_by_value(map<string, int> const &data, int value) {
    auto it = find_if(data.begin(), data.end(), [value](const pair<string, int> &item) -> bool {
        return item.second == value;
    });
    return *it;
}

pair<string, int> find_by_key(map<string, int> const &data, const string &key) {
    auto it = find_if(data.begin(), data.end(), [key](const pair<string, int> &item) -> bool {
        return item.first == key;
    });
    return *it;
}

map<string, int> filter(map<string, int> const &data) {
    map<string, int> result;
    for (auto const &item: data) {
        if (item.second > threshold) {
            result[item.first] = item.second;
        }
    }
    return result;
}

template<typename T>
void print_queue(T &q) {
    while (!q.empty()) {
        cout << q.top() << " ";
        q.pop();
    }
    cout << endl;
}

template<class T>
class Node {
protected:
    // закрытые переменные Node N; N.data = 10 вызовет ошибку
    T data;
    // не можем хранить Node, но имеем право хранить указатель
    Node *left;
    Node *right;
    Node *parent;
    // переменная, необходимая для поддержания баланса дерева
    int height;
public:
    // доступные извне переменные и функции
    virtual void setData(T d) { data = d; }

    virtual T getData() { return data; }

    int getHeight() { return height; }

    virtual Node *getLeft() { return left; }

    virtual Node *getRight() { return right; }

    virtual Node *getParent() { return parent; }

    virtual void setLeft(Node *N) { left = N; }

    virtual void setRight(Node *N) { right = N; }

    virtual void setParent(Node *N) { parent = N; }

    // Конструктор. Устанавливаем стартовые значения для указателей
    explicit Node<T>(T n) {
        data = n;
        left = right = parent = NULL;
        height = 1;
    }

    Node() {
        left = nullptr;
        right = nullptr;
        parent = nullptr;
        data = 0;
        height = 1;
    }

    virtual void print() {
        cout << data << endl;
    }

    virtual void setHeight(int h) {
        height = h;
    }

    template<class U>
    friend ostream &operator<<(ostream &stream, Node<T> &N);
};

template<class T>
ostream &operator<<(ostream &stream, Node<T> &N) {
    stream << "Node data: " << N.getData() << ", height: " << N.getHeight() << endl;
    return stream;
}

template<class T>
void print(Node<T> *N) {
    cout << N->getData() << endl;
}

template<class T>
class Tree {
protected:
    // корень - его достаточно для хранения всего дерева
    Node<T> *root;
public:
    // доступ к корневому элементу
    virtual Node<T> *getRoot() { return root; }

    // конструктор дерева: в момент создания дерева ни одного узла нет, корень смотрит в никуда
    Tree() { root = nullptr; }

    // Рекуррентная функция добавления узла. Устроена аналогично, но вызывает сама себя - добавление в левое или правое поддерево
    virtual Node<T> *Add_R(Node<T> *N) {
        return Add_R(N, root);
    }

    virtual Node<T> *Add_R(Node<T> *N, Node<T> *Current) {
        if (N == nullptr) return nullptr;
        if (root == nullptr) {
            root = N;
            return N;
        }
        // идем влево
        if (Current->getData() > N->getData()) {
            if (Current->getLeft() != nullptr) Current->setLeft(Add_R(N, Current->getLeft()));
            else Current->setLeft(N);
            Current->getLeft()->setParent(Current);
        }
        // идем вправо
        if (Current->getData() < N->getData()) {
            if (Current->getRight() != nullptr) Current->setRight(Add_R(N, Current->getRight()));
            else Current->setRight(N);
            Current->getRight()->setParent(Current);
        }
        // нашли совпадение
        if (Current->getData() == N->getData()) return Current;
        // для несбалансированного дерева поиска
        return Current;
    }

    // Функция для добавления числа. Делаем новый узел с этими данными и вызываем нужную функцию добавления в дерево
    virtual void Add(int n) {
        auto *N = new Node<T>;
        N->setData(n);
        Add_R(N);
    }

    virtual Node<T> *Min(Node<T> *Current = nullptr) {
        // Минимум - это самый "левый" узел. Идём по дереву всегда влево
        if (root == nullptr) return nullptr;
        if (Current == nullptr) Current = root;
        while (Current->getLeft() != nullptr) Current = Current->getLeft();
        return Current;
    }

    virtual Node<T> *Max(Node<T> *Current = nullptr) {
        // Минимум - это самый "правый" узел. Идём по дереву всегда вправо
        if (root == nullptr) return nullptr;
        if (Current == nullptr) Current = root;
        while (Current->getRight() != nullptr) Current = Current->getRight();
        return Current;
    }

    // Поиск узла в дереве. Второй параметр - в каком поддереве искать, первый - что искать
    virtual Node<T> *Find(int data, Node<T> *Current) {
        // база рекурсии
        if (Current == nullptr) return nullptr;
        if (Current->getData() == data) return Current;
        // рекурсивный вызов
        if (Current->getData() > data) return Find(data, Current->getLeft());
        if (Current->getData() < data) return Find(data, Current->getRight());
        return nullptr;
    }

    // три обхода дерева
    virtual void PreOrder(Node<T> *N, void (*f)(Node<T> *)) {
        if (N != nullptr) f(N);
        if (N != nullptr && N->getLeft() != nullptr) PreOrder(N->getLeft(), f);
        if (N != nullptr && N->getRight() != nullptr) PreOrder(N->getRight(), f);
    }

    // InOrder-обход даст отсортированную последовательность
    virtual void InOrder(Node<T> *N, void (*f)(Node<T> *)) {
        if (N != nullptr && N->getLeft() != nullptr) InOrder(N->getLeft(), f);
        if (N != nullptr) f(N);
        if (N != nullptr && N->getRight() != nullptr) InOrder(N->getRight(), f);
    }

    virtual void PostOrder(Node<T> *N, void (*f)(Node<T> *)) {
        if (N != nullptr && N->getLeft() != nullptr) PostOrder(N->getLeft(), f);
        if (N != nullptr && N->getRight() != nullptr) PostOrder(N->getRight(), f);
        if (N != nullptr) f(N);
    }
};


template<class T>
class Heap {
private:
    // массив
    Node<T> *arr;
    // сколько элементов добавлено
    int len;
    //  сколько памяти выделено
    int size;
public:
    // доступ к вспомогательным полям кучи и оператор индекса
    int getCapacity() { return size; }

    int getCount() { return len; }

    Node<T> &operator[](int index) {
        if (index < 0 || index >= len); // проверка на соответствие индекса границам
        return arr[index];
    }

    // конструктор
    explicit Heap<T>(int MemorySize = 100) {
        arr = new Node<T>[MemorySize];
        len = 0;
        size = MemorySize;
    }

    // поменять местами элементы arr[index1], arr[index2]
    void Swap(int index1, int index2) {
        if (index1 <= 0 || index1 >= len);
        if (index2 <= 0 || index2 >= len);
        // здесь нужна защита от дурака
        Node<T> temp = arr[index1];
        arr[index1] = arr[index2];
        arr[index2] = temp;
    }

    // скопировать данные между двумя узлами
    void Copy(Node<T> *dest, Node<T> *source) {
        dest->setValue(source->getValue());
    }

    //  функции получения левого, правого дочернего элемента, родителя или их индексов в массиве
    Node<T> *GetLeftChild(int index) {
        if (index < 0 || index * 2 >= len); // здесь нужна защита от дурака
        return &arr[index * 2 + 1];
    }

    Node<T> *GetRightChild(int index) {
        if (index < 0 || index * 2 >= len); // здесь нужна защита от дурака
        return &arr[index * 2 + 2];
    }

    Node<T> *GetParent(int index) {
        if (index <= 0 || index >= len); //здесь нужна защита от дурака
        if (index % 2 == 0) return &arr[index / 2 - 1];
        return &arr[index / 2];
    }

    int GetLeftChildIndex(int index) {
        if (index < 0 || index * 2 >= len); //здесь нужна защита от дурака
        return index * 2 + 1;
    }

    int GetRightChildIndex(int index) {
        if (index < 0 || index * 2 >= len); //здесь нужна защита от дурака
        return index * 2 + 2;
    }

    int GetParentIndex(int index) {
        if (index <= 0 || index >= len); //здесь нужна защита от дурака
        if (index % 2 == 0) return index / 2 - 1;
        return index / 2;
    }

    // просеять элемент вверх
    void SiftUp(int index = -1) {
        if (index == -1) index = len - 1;
        auto parent = GetParentIndex(index);
        auto index2 = GetLeftChildIndex(parent);
        if (index2 == index) index2 = GetRightChildIndex(parent);
        auto max_index = index;
        if (index < len && index2 < len && parent >= 0) {
            if (arr[index] > arr[index2]) max_index = index;
            if (arr[index] < arr[index2]) max_index = index2;
        }
        // нужно просеивание вверх
        if (parent < len && parent >= 0 && arr[max_index] > arr[parent]) {
            Swap(parent, max_index);
            SiftUp(parent);
        }
    }

    // добавление элемента - вставляем его в конец массива и просеиваем вверх
    template<typename D>
    void Add(D v) {
        auto *N = new Node<D>;
        N->setValue(v);
        Add(N);
    }

    template<class D>
    void Add(Node<D> *N) {
        if (len < size) {
            Copy(&arr[len], N);
            len++;
            SiftUp();
        }
    }

    // перечислить элементы кучи и применить к ним функцию
    void Straight(void(*f)(Node<T> *)) {
        for (auto i = 0; i < len; i++) {
            f(&arr[i]);
        }
    }

    // перебор элементов, аналогичный проходам бинарного дерева
    void InOrder(void(*f)(Node<T> *), int index = 0) {
        if (GetLeftChildIndex(index) < len) PreOrder(f, GetLeftChildIndex(index));
        if (index >= 0 && index < len) f(&arr[index]);
        if (GetRightChildIndex(index) < len) PreOrder(f, GetRightChildIndex(index));
    }
};

int main() {
    // Task 1
    map<string, int> marks;
    marks["Petrov"] = 5;
    marks["Ivanov"] = 4;
    marks["Sidorov"] = 5;
    marks["Nikolaev"] = 3;
    marks["Abramov"] = 4;
    marks["Fedorov"] = 5;
    marks["Kuznetsov"] = 4;

    for (auto const &item: marks) {
        cout << item.first << ": " << item.second << endl;
    }

    auto value = find_by_value(marks, 3);
    cout << value.first << ": " << value.second << endl;
    auto value2 = find_by_key(marks, "Ivanov");
    cout << value2.first << ": " << value2.second << endl << endl;

    auto result = filter(marks);
    for (auto const &item: result) {
        cout << item.first << ": " << item.second << endl;
    }

    // Task 2

    priority_queue<int> q;
    for (int n: {1, 8, 5, 6, 3, 4, 0, 9, 7, 2}) q.push(n);
    print_queue(q);

    // Task 3
    Tree<double> T;
    int arr[15];
    for (int i = 0; i < 15; i++) arr[i] = (int) (100 * cos(15 * double(i + 1)));
    for (int i: arr) T.Add(i);
    Node<double> *M = T.Min();
    cout << "Min = " << M->getData() << '\t' << "Find " << arr[3] << ": " << endl << *(T.Find(arr[3], T.getRoot()))
         << endl;
    void (*f_ptr)(Node<double> *);
    f_ptr = print;
    cout << "-----" << endl << "Inorder:";
    T.InOrder(T.getRoot(), f_ptr);
    cout << endl;
    return 0;
}
