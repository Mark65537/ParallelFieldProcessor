#include <windows.h>
#include <iostream>

using namespace std;
// �������� �������, ������� ����� ����������� � ������
DWORD WINAPI ProcessField(LPVOID lpParam);

const int fieldCount = 3; // ���������� �����
int fieldArrays[fieldCount][100]; // ������� ������� ��� ������� ����
HANDLE hThreads[fieldCount];
CRITICAL_SECTION cs; // ����������� ������

int main() {
    // ������������� ����������� ������
    InitializeCriticalSection(&cs);

    // ������������� ������ ��� ������� ���� (������)
    for (int i = 0; i < fieldCount; ++i) {
        for (int j = 0; j < 100; ++j) {
            fieldArrays[i][j] = j; // ������ ������������� �������
        }

        // �������� ������ ��� ��������� ����
        hThreads[i] = CreateThread(
            NULL, 0, ProcessField, (LPVOID)i, 0, NULL
        );

        if (hThreads[i] == NULL) {
            cout << "Failed to create thread for field " << i + 1 << endl;
        }
    }

    // �������� ���������� ���� �������
    WaitForMultipleObjects(fieldCount, hThreads, TRUE, INFINITE);

    // ������������ ��������
    for (int i = 0; i < fieldCount; ++i) {
        CloseHandle(hThreads[i]);
    }

    // �������� ����������� ������
    DeleteCriticalSection(&cs);

    return 0;
}

// ������� ��� ��������� ����, ����������� � ������
DWORD WINAPI ProcessField(LPVOID lpParam) {
    int fieldNumber = (int)lpParam;
    int* fieldArray = fieldArrays[fieldNumber]; // ��������� �� ������ �������

    // ��������� ������� ������� (������)
    for (int i = 0; i < 100; ++i) {
        fieldArray[i] *= 2; // ������ ���������
    }

    // ���� � ����������� ������
    EnterCriticalSection(&cs);

    cout << "Field " << fieldNumber + 1 << " processed." << endl;

    // ����� �� ����������� ������
    LeaveCriticalSection(&cs);

    return 0;
}

