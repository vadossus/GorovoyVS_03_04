#include <iostream>
#include <string>
#include <cstdio>

using namespace std;

const int MAX_CARS = 100; 

class Car {
private:
    string brand;
    string gos_number;
    int year;
    double price;

public:
    Car() : year(0), price(0.0) {}

    Car(const string& b, const string& lp, int y, double p)
        : brand(b), gos_number(lp), year(y), price(p) {
    }

    string getbrand() const { return brand; }
    string get_gos_number() const { return gos_number; }
    int getyear() const { return year; }
    double getprice() const { return price; }

    void set_brand(const string& b) { brand = b; }
    void set_gos_number(const string& lp) { gos_number = lp; }
    void setyear(int y) { year = y; }
    void setprice(double p) { price = p; }

    void print() const {
        cout << "Марка: " << brand
            << ", Номер: " << gos_number
            << ", Год: " << year
            << ", Стоимость: " << price << endl;
    }

    void write_file(FILE* file) const {
        fprintf_s(file, "%s;%s;%d;%.2f\n", brand.c_str(), gos_number.c_str(), year, price);
    }

    static Car read_file(FILE* file) {
        Car car;
        char brand[100], gos_number[20];
        int year;
        double price;

        if (fscanf_s(file, "%99[^;];%19[^;];%d;%lf\n", brand, (unsigned)_countof(brand),
            gos_number, (unsigned)_countof(gos_number), &year, &price) == 4) {
            car.set_brand(brand);
            car.set_gos_number(gos_number);
            car.setyear(year);
            car.setprice(price);
        }

        return car;
    }
};

void add_car_file(const string& filename) {
    Car car;
    string input;
    int year;
    double price;

    cout << "\nДобавление нового автомобиля:" << endl;

    cout << "Марка: ";
    getline(cin, input);
    car.set_brand(input);

    cout << "Номерной знак: ";
    getline(cin, input);
    car.set_gos_number(input);

    cout << "Год выпуска: ";
    cin >> year;
    car.setyear(year);

    cout << "Стоимость: ";
    cin >> price;
    car.setprice(price);
    cin.ignore(); 

    FILE* file;
    if (fopen_s(&file, filename.c_str(), "a") == 0) {
        car.write_file(file);
        fclose(file);
        cout << "Автомобиль успешно добавлен в файл!" << endl;
    }
    else {
        cerr << "Ошибка открытия файла для записи!" << endl;
    }
}

int read_cars_file(const string& filename, Car cars[]) {
    int count = 0;
    FILE* file;

    if (fopen_s(&file, filename.c_str(), "r") == 0) {
        while (!feof(file) && count < MAX_CARS) {
            Car car = Car::read_file(file);
            if (!car.getbrand().empty()) {
                cars[count++] = car;
            }
        }
        fclose(file);
    }
    else {
        cerr << "Ошибка открытия файла для чтения!" << endl;
    }

    return count;
}

void write_cars_file(const string& filename, Car cars[], int count) {
    FILE* file;
    if (fopen_s(&file, filename.c_str(), "w") == 0) {
        for (int i = 0; i < count; i++) {
            cars[i].write_file(file);
        }
        fclose(file);
    }
    else {
        cerr << "Ошибка открытия файла для записи!" << endl;
    }
}

void print_cars(Car cars[], int count) {
    if (count == 0) {
        cout << "Нет данных об автомобилях." << endl;
        return;
    }

    cout << "\nСписок автомобилей:" << endl;
    for (int i = 0; i < count; i++) {
        cars[i].print();
    }
}

void sort_save_cars(const string& filename) {
    Car cars[MAX_CARS];
    int count = read_cars_file(filename, cars);
    if (count == 0) return;

    cout << "\nВыберите критерий сортировки:" << endl;
    cout << "1. По марке (возрастание)" << endl;
    cout << "2. По марке (убывание)" << endl;
    cout << "3. По году выпуска (возрастание)" << endl;
    cout << "4. По году выпуска (убывание)" << endl;
    cout << "5. По стоимости (возрастание)" << endl;
    cout << "6. По стоимости (убывание)" << endl;
    cout << "0. Отмена" << endl;

    int choice;
    cin >> choice;
    cin.ignore();

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            bool swap = false;

            switch (choice) {
            case 1:
                swap = cars[j].getbrand() > cars[j + 1].getbrand();
                break;
            case 2:
                swap = cars[j].getbrand() < cars[j + 1].getbrand();
                break;
            case 3:
                swap = cars[j].getyear() > cars[j + 1].getyear();
                break;
            case 4:
                swap = cars[j].getyear() < cars[j + 1].getyear();
                break;
            case 5:
                swap = cars[j].getprice() > cars[j + 1].getprice();
                break;
            case 6:
                swap = cars[j].getprice() < cars[j + 1].getprice();
                break;
            case 0:
                return;
            default:
                cout << "Неверный выбор!" << endl;
                return;
            }

            if (swap) {
                Car temp = cars[j];
                cars[j] = cars[j + 1];
                cars[j + 1] = temp;
            }
        }
    }

    write_cars_file(filename, cars, count);
    cout << "Данные успешно отсортированы и сохранены!" << endl;
    print_cars(cars, count);
}

int main() {
    setlocale(LC_ALL, "Russian");
    const string filename = "cars.txt";

    while (true) {
        cout << "\nМеню:" << endl;
        cout << "1. Добавить автомобиль" << endl;
        cout << "2. Просмотреть все автомобили" << endl;
        cout << "3. Отсортировать и сохранить" << endl;
        cout << "0. Выход" << endl;
        cout << "Выберите действие: ";

        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            add_car_file(filename);
            break;
        case 2: {
            Car cars[MAX_CARS];
            int count = read_cars_file(filename, cars);
            print_cars(cars, count);
            break;
        }
        case 3:
            sort_save_cars(filename);
            break;
        case 0:
            return 0;
        default:
            cout << "Неверный выбор!" << endl;
        }
    }

    return 0;
}