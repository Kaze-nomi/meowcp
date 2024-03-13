Сгенерировать мейк-файлы:

cmake -B./build -S.

Скомпелировать все проекты:

cmake --build build

Запуск тестов:

ctest --test-dir ./build
