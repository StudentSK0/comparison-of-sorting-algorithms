# Сравнение алгоритмов сортировки

*Цель работы* -- сравнить стандартные и адаптированные алгоритмы сортировки строк и определить, как использование префиксной структуры строк влияет на время работы и количество посимвольных сравнений.

## Структура репозитория

```
project/
├── src/
│   ├── main.cpp
│   ├── string_generator.cpp
│   ├── string_sort_algorithms.cpp
│   └── string_sort_tester.cpp
├── include/
│   ├── string_generator.h
│   ├── string_sort_algorithms.h
│   └── string_sort_tester.h
├── scripts/
│   └── plot_results.py
├── results/
├── data/
├── tex/
├── Makefile
├── requirements.txt
├── README.md
└── .gitignore
```


## Сборка и запуск
**Сборка производится через Makefile:**

```bash
make
```

Исполняемый файл: `./string_sort_analysis`.

**Запуск:**

```bash
make run
```

## Построение графиков

```bash
make plots
```

Команда  создаёт локальный Python-интерпретатор в `.venv/` (если его ещё нет). Затем устанавливает зависимости 
из `requirements.txt` и запускает `scripts/plot_results.py`.
# comparison-of-sorting-algorithms
