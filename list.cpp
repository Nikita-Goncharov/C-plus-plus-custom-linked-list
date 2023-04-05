#include <iostream>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

// TODO: Понять как работает это
using namespace std;
using json = nlohmann::json;


class Node {
    public:
        string brand, model; 
        float liters;
        int numberOfSeats;
        bool isOnTheRun;
        Node *next = 0;

        Node(string brandFromJson, string modelFromJson, float litersFromJson, int numberOfSeatsFromJson, bool isOnTheRunFromJson)
        {
            brand = brandFromJson;
            model = modelFromJson;
            liters = litersFromJson;
            numberOfSeats = numberOfSeatsFromJson;
            isOnTheRun = isOnTheRunFromJson;
        }

        static void printList(Node *head)
        {
            Node *currentNode = head;

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

            while (currentNode)
            {
                if (i == index)
                {
                    cout << "We are here" << endl;
                    cout << currentNode << endl;
                    cout << "Brand: " << currentNode->brand << endl;
                    cout << "Model: " << currentNode->model << endl;
                    cout << "Liters: " << currentNode->liters << endl;
                    cout << "Number of seats: " << currentNode->numberOfSeats << endl;
                    cout << "Is on the run: " << (currentNode->isOnTheRun ? "true" : "false") << endl;
                    cout << endl;
                    break; // Если элемент был выведен, то нам уже нету смысла продолжать цикл
                    i++;
                }
                currentNode = currentNode->next;
            }
        }

        static void deleteList(Node *head)
        {
            Node *currentNode;

            while (head)
            {
                currentNode = head;
                head = head->next;
                delete currentNode;
            }
        }

        static void deleteNodeByIndex(Node *head, int index = -1)
        {
            if (head == NULL)
            {
                cout << "List is empty." << endl;
                return;
            }

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
            if (index == 0)
            {
                Node *temp = head;
                head = head->next;
                delete temp;
                return;
            }
            Node *current = head;
            Node *prev = NULL;
            int count = 0;
            while (current != NULL && count != index)
            {
                prev = current;
                current = current->next;
                count++;
            }
            if (current == NULL)
            {
                cout << "Index out of range." << endl;
                return;
            }
            prev->next = current->next;
            delete current;
        }

        // static void addElement(Node *head, int index = -1)
        // {
        //     // По дефолту добавляет елемент в конец
        //     Node *currentNode;

        //     string brand, model;
        //     float liters;
        //     int numberOfSeats;
        //     bool isOnTheRun;

        //     if (index == -1) {
        //         while(head) {
        //             currentNode = head;
        //             if (head->next == NULL) {

        //             }
        //             head = head->next;

        //         }
        //     } else {
        //         for (int i = 0; i < index; i++)
        //         {
        //             currentNode = head;
        //             head = head->next;
        //         }
        //     }
            
        // }

        static void addNodeByIndex(Node *head, int index = -1)
        {
            Node *newNode = new Node("brand", "model", 3.6, 4, true);
            if (index < -1)
            {
                cout << "Index out of range." << endl;
                return;
            }
            if (index == -1 || head == NULL)
            {
                if (head == NULL)
                {
                    head = newNode;
                }
                else
                {
                    Node *current = head;
                    while (current->next != NULL)
                    {
                        current = current->next;
                    }
                    current->next = newNode;
                }
                return;
            }
            if (index == 0)
            {
                newNode->next = head;
                head = newNode;
                return;
            }
            Node *current = head;
            int count = 0;
            while (current != NULL && count != index - 1)
            {
                current = current->next;
                count++;
            }
            if (current == NULL)
            {
                cout << "Index out of range." << endl;
                return;
            }
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
    int menuNumber, amount, index;
    string currentCarKey;
    json carsJson, car;
    Node *head = 0, *now;

    // Парсим файл возвращаем данные, если ошибка - null
    carsJson = parseJsonCars("cars.json");


    // Проверка на ошибку парсинга файла
    if (carsJson == NULL) {
            return 0;
    }

    // Берём кол-во елементов в json-файле для создания такого же количества елементов в списке
    amount = carsJson.size();


    // Создание и заполнение списка данными из файла 
    for (int i = 0; i < amount; i++)
    {
        currentCarKey = "car" + to_string(i+1);
        car = carsJson[currentCarKey]; 
        if (head == 0)
        {
            head = new Node(car["brand"], car["model"], car["liters"], car["numberOfSeats"], car["isOnTheRun"]); // TODO: Убрать повторы
            now = head;
        }
        else
        {
            now->next = new Node(car["brand"], car["model"], car["liters"], car["numberOfSeats"], car["isOnTheRun"]);
            now = now->next;
        }
        
    }

    // Меню программы
    while (true) {
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
        if (menuNumber < 1 || menuNumber > 7) {
            cout << "Please, write correct number" << endl;
            continue;
        }

        // Если человек хочет закончить 
        if (menuNumber == 7) {
            break;
        }

        // Действия в зависимости от того что выбрал человек
        switch (menuNumber)
        {
        case 1:
            Node::printList(head);
            break;
        case 2:
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
            Node::addNodeByIndex(head, 1);
            break;
        case 6:
            Node::deleteNodeByIndex(head, 1);
            break;
        }
    }

    // Удаление происходит всегда 
    Node::deleteList(head);
    return 0;
}