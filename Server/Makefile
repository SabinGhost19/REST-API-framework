# Definirea variabilelor
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g # Adăugat -g pentru debugging
SOURCES = main.cpp HTTPServer.cpp Request.cpp Response.cpp Router.cpp Utils.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = main

# Regula principală pentru a construi executabilul
all: $(TARGET)

# Regula pentru a construi executabilul din obiecte
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regula pentru a compila fișierele sursă în fișiere obiect
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regula pentru curățare (ștergerea fișierelor obiect și a executabilului)
clean:
	rm -f $(OBJECTS) $(TARGET)

# Regula pentru a afişa informaţii despre utilizare
.PHONY: all clean
