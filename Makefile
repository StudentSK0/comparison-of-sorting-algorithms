CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Iinclude
VENV_DIR ?= .venv
PYTHON ?= $(VENV_DIR)/bin/python
REQUIREMENTS ?= requirements.txt

SRC = src/main.cpp \
      src/string_generator.cpp \
      src/string_sort_algorithms.cpp \
      src/string_sort_tester.cpp

TARGET = string_sort_analysis

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

$(VENV_DIR)/bin/python:
	python3 -m venv $(VENV_DIR)

deps: $(VENV_DIR)/bin/python
	$(PYTHON) -m pip install -r $(REQUIREMENTS)

plots: deps
	$(PYTHON) scripts/plot_results.py

clean:
	rm -f $(TARGET) *.o *.out

.PHONY: all run deps plots clean
