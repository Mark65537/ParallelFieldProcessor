#include <windows.h>
#include <iostream>

using namespace std;
// Прототип функции, которая будет выполняться в потоке
DWORD WINAPI ProcessField(LPVOID lpParam);

const int fieldCount = 3; // Количество полей
int fieldArrays[fieldCount][100]; // Массивы посевов для каждого поля
HANDLE hThreads[fieldCount];
CRITICAL_SECTION cs; // Критическая секция

int main() {
    // Инициализация критической секции
    InitializeCriticalSection(&cs);

    // Инициализация данных для каждого поля (пример)
    for (int i = 0; i < fieldCount; ++i) {
        for (int j = 0; j < 100; ++j) {
            fieldArrays[i][j] = j; // Пример инициализации массива
        }

        // Создание потока для обработки поля
        hThreads[i] = CreateThread(
            NULL, 0, ProcessField, (LPVOID)i, 0, NULL
        );

        if (hThreads[i] == NULL) {
            cout << "Failed to create thread for field " << i + 1 << endl;
        }
    }

    // Ожидание завершения всех потоков
    WaitForMultipleObjects(fieldCount, hThreads, TRUE, INFINITE);

    // Освобождение ресурсов
    for (int i = 0; i < fieldCount; ++i) {
        CloseHandle(hThreads[i]);
    }

    // Удаление критической секции
    DeleteCriticalSection(&cs);

    return 0;
}

// Функция для обработки поля, выполняемая в потоке
DWORD WINAPI ProcessField(LPVOID lpParam) {
    int fieldNumber = (int)lpParam;
    int* fieldArray = fieldArrays[fieldNumber]; // Указатель на массив посевов

    // Обработка массива посевов (пример)
    for (int i = 0; i < 100; ++i) {
        fieldArray[i] *= 2; // Пример обработки
    }

    // Вход в критическую секцию
    EnterCriticalSection(&cs);

    cout << "Field " << fieldNumber + 1 << " processed." << endl;

    // Выход из критической секции
    LeaveCriticalSection(&cs);

    return 0;
}

