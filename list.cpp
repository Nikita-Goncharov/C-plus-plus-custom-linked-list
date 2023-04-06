#include <iostream>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

using namespace std;
// Подключение пространства имен
// Мои комментарии: тут мы подключаем пространство json из библиотеки nlohmann, чтобы не юзать nlohmann полностью
using json = nlohmann::json;

class Node {
private:
    string brand, model;
    float liters;
    int numberOfSeats;
    bool isOnTheRun;

public:
    Node *next = 0;

    Node(string brandFromJson, string modelFromJson, float litersFromJson, int numberOfSeatsFromJson, bool isOnTheRunFromJson)
    {
        // Заполняем элемент
        brand = brandFromJson;
        model = modelFromJson;
        liters = litersFromJson;
        numberOfSeats = numberOfSeatsFromJson;
        isOnTheRun = isOnTheRunFromJson;
    }

        static void printList(Node *head)
        {
            Node *currentNode = head;

            // Проходимся по всем элементам и выводим их
            while (currentNode)
            {
                cout << "Brand: " << currentNode->brand << endl;
                cout << "Model: " << currentNode->model << endl;
                cout << "Liters: " << currentNode->liters << endl;
                cout << "Number of seats: " << currentNode->numberOfSeats << endl;
                cout << "Is on the run: " << (currentNode->isOnTheRun ? "true" : "false") << endl;
                currentNode = currentNode->next;
                cout << endl;
            }
        }

        static void printElementOfListByNum(Node *head, int index)
        {
            int i = 0;
            Node *currentNode = head;

            // Проходим по элементов до нужного по индексу
            while (currentNode != NULL && i != index)
            {
                currentNode = currentNode->next;
                i++;
            }
            // Если элемента нету, то значит передан неправильный индекс
            if (currentNode == NULL)
            {
                cout << "Index out of range." << endl;
                return;
            }
            // Иначе выводим элемент
            cout << "Brand: " << currentNode->brand << endl;
            cout << "Model: " << currentNode->model << endl;
            cout << "Liters: " << currentNode->liters << endl;
            cout << "Number of seats: " << currentNode->numberOfSeats << endl;
            cout << "Is on the run: " << (currentNode->isOnTheRun ? "true" : "false") << endl;
            cout << endl;
        }

        static void deleteList(Node *head)
        {
            Node *currentNode;
            // Проходим по всем элементам и удаляем их
            while (head)
            {
                currentNode = head;
                head = head->next;
                delete currentNode;
            }
        }

        static void deleteNodeByIndex(Node *head, int index = -1)
        {
            // Проверка на пустой список
            if (head == NULL)
            {
                cout << "List is empty." << endl;
                return;
            }
            // Проверка на дефолтное значение, если -1 - удаляем последний элемент списка
            if (index == -1) {
                Node *currentNode;

                while (head)
                {
                    currentNode = head;
                    
                    head = head->next;
                    if (head->next == NULL)
                    {
                        delete head;
                        currentNode->next = NULL;
                        return;
                    }
                }
            }
            // Если индекс 0 - удаляем первый элемент
            if (index == 0)
            {
                Node *temp = head;
                head = head->next;
                delete temp;
                return;
            }
            // Иначе проходимся по списку до нужного элемента
            Node *current = head;
            Node *prev = NULL;
            int count = 0;
            while (current != NULL && count != index)
            {
                prev = current;
                current = current->next;
                count++;
            }
            // Если элемента нету, то значит передан неправильный индекс
            if (current == NULL)
            {
                cout << "Index out of range." << endl;
                return;
            }
            // Меняем ссылку предыдущего элемента на следующий, обходя элемент который мы удаляем
            prev->next = current->next;
            delete current;
        }

        static void addNodeByIndex(Node *&head, int index = -1)
        {
            int numberOfSeats;
            float liters;
            bool isOnRun = true;
            string brand, model;

            cout << "Brand: ";
            cin >> brand;
            cout << "Model: ";
            cin >> model;
            cout << "Liters: ";
            cin >> liters;
            cout << "Number of seats: ";
            cin >> numberOfSeats;
            cout << "Is on the run: ";

            // Создаём новый элемент
            Node *newNode = new Node(brand, model, liters, numberOfSeats, isOnRun);
            // Проверка на корректность индекса
            if (index < -1)
            {
                cout << "Index out of range." << endl;
                return;
            }
            // Проверка на дефолтное значение, если -1 - добавляем последним элементом списка
            if (index == -1)
            {
                Node *current = head;
                while (current->next != NULL)
                {
                    current = current->next;
                }
                current->next = newNode;
                return;
            }
            // Если индекс 0 - добавляем первым элементом
            if (index == 0)
            {
                newNode->next = head;
                head = newNode;
                return;
            }
            // Иначе проходим по списку и вставляем в нужное место
            Node *current = head;
            int count = 0;
            while (current != NULL && count != index - 1)
            {
                current = current->next;
                count++;
            }
            // Если элемента нету, то значит передан неправильный индекс
            if (current == NULL)
            {
                cout << "Index out of range." << endl;
                return;
            }
            // Меняем ссылки чтобы вставить новый элемент
            newNode->next = current->next;
            current->next = newNode;
        }
};


// Функция парсинга json`а, если всё ОК - данные, если нет - null
json parseJsonCars(string path)
{
        json carsJson;

        ifstream file(path);
        if (!file.is_open())
        {
            cout << "Failed to open file" << endl;
            return carsJson;
        }
        // Чтение данных из файла
        file >> carsJson;
        file.close();
        return carsJson;
}


int main()
{
        int menuNumber, amount, index, numberOfSeats;
        float liters;
        bool isOnRun = true;
        string currentCarKey, brand, model;
        json carsJson, car;
        Node *head = 0, *now, *element;

        // Парсим файл возвращаем данные, если ошибка - null
        carsJson = parseJsonCars("cars.json");

        // Проверка на ошибку парсинга файла
        if (carsJson == NULL)
        {
            return 0;
        }

        // Берём кол-во элементов в json-файле для создания такого же количества элементов в списке
        amount = carsJson.size();

        // Создание и заполнение списка данными из файла
        for (int i = 0; i < amount; i++)
        {
            currentCarKey = "car" + to_string(i + 1);
            car = carsJson[currentCarKey];
            element = new Node(car["brand"], car["model"], car["liters"], car["numberOfSeats"], car["isOnTheRun"]);
            if (head == 0)
            {
                head = element;
                now = head;
            }
            else
            {
                now->next = element;
                now = now->next;
            }
        }

        // Меню программы
        while (true)
        {
            cout << "Menu: " << endl;
            cout << "1- Print all list" << endl;
            cout << "2- Print element of list by number(started by 0)" << endl;
            cout << "3- Add element to end in list" << endl;
            cout << "4- Remove element from end in list" << endl;
            cout << "5- Add element by position in list" << endl;
            cout << "6- Remove element by position in list" << endl;
            cout << "7- Quit" << endl;
            cout << "Your choise: ";
            cin >> menuNumber;

            // Проверка на корректность введеного числа
            if (menuNumber < 1 || menuNumber > 7)
            {
                cout << "Please, write correct number" << endl;
                continue;
            }

            // Если человек хочет закончить
            if (menuNumber == 7)
            {
                break;
            }

            // Действия в зависимости от того что выбрал человек
            switch (menuNumber)
            {
            case 1:
                Node::printList(head);
                break;
            case 2:
                cout << endl;
                cout << "Write index of element which you want print: ";
                cin >> index;

                Node::printElementOfListByNum(head, index);
                cout << endl;
                break;
            case 3:
                Node::addNodeByIndex(head);
                break;
            case 4:
                Node::deleteNodeByIndex(head);
                break;
            case 5:
                cout << "Write index where you want insert element: ";
                cin >> index;

                Node::addNodeByIndex(head, index = index);
                break;
            case 6:
                cout << "Write index of element which you want delete: ";
                cin >> index;

                Node::deleteNodeByIndex(head, index = index);
                break;
            }
        }

        // Удаление списка происходит всегда
        Node::deleteList(head);
        return 0;
}