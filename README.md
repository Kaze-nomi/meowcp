Сгенерировать мейк-файлы:

cmake -B./build -S.

Скомпелировать все проекты:

cmake --build build

Запуск тестов:

ctest --test-dir ./build

Рассчёт первых 40 чисел Фибоначчи:

./build/MeowCPU/MeowCPU ./assembly/fibonachi.meow 
