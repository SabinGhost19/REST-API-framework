# Definirea variabilelor
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# Include directoarele PostgreSQL și link cu biblioteca libpq, eliminând specificațiile problematice
LIBPQ_FLAGS = $(shell pg_config --cflags | sed 's|-specs=/usr/lib/rpm/redhat/redhat-hardened-cc1||g' | sed 's|-specs=/usr/lib/rpm/redhat/redhat-annobin-cc1||g')
LIBPQ_LIBS = $(shell pg_config --libs | sed 's|-lpgcommon||g' | sed 's|-lpgport||g' | sed 's|-lgssapi_krb5||g')

# Include directoarele pentru toate modulele
INCLUDES = -I. -Imodels -Irepository/userRepository

# Specifică fișierele sursă și directoarele de include
SOURCES = main.cpp HTTPServer.cpp Request.cpp Response.cpp Router.cpp Utils.cpp ThreadPool.cpp MiddleWare.cpp \
          models/PostgresDB.cpp models/repository/userRepository/UserRepository.cpp

# Convertim fișierele sursă în fișiere obiect
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = main

# Regula principală pentru a construi executabilul
all: $(TARGET)

# Regula pentru a construi executabilul din obiecte
$(TARGET): $(OBJECTS)
	@echo "Linking executable $(TARGET)"
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBPQ_FLAGS) -o $@ $^ $(LIBPQ_LIBS) -lpq

# Regula pentru a compila fișierele sursă în fișiere obiect
%.o: %.cpp
	@echo "Compiling $<"
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBPQ_FLAGS) -c $< -o $@

# Regula pentru curățare (ștergerea fișierelor obiect și a executabilului)
clean:
	@echo "Cleaning up..."
	rm -f $(OBJECTS) $(TARGET)

# Regula pentru a afișa informații despre utilizare
.PHONY: all clean
