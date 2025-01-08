CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

LIBPQ_FLAGS = $(shell pg_config --cflags | sed 's|-specs=/usr/lib/rpm/redhat/redhat-hardened-cc1||g' | sed 's|-specs=/usr/lib/rpm/redhat/redhat-annobin-cc1||g')
LIBPQ_LIBS = $(shell pg_config --libs | sed 's|-lpgcommon||g' | sed 's|-lpgport||g' | sed 's|-lgssapi_krb5||g')

INCLUDES = -I. -Imodels -Irepository/userRepository -I./bcrypt

SOURCES = main.cpp HTTPServer.cpp Request.cpp Response.cpp Router.cpp Utils.cpp ThreadPool.cpp MiddleWare.cpp \
          models/PostgresDB.cpp models/repository/userRepository/UserRepository.cpp

OBJECTS = $(SOURCES:.cpp=.o)
TARGET = main

BCRYPT_LIB = ./bcrypt/bcrypt.a

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "Linking executable $(TARGET)"
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBPQ_FLAGS) -o $@ $^ $(LIBPQ_LIBS) $(BCRYPT_LIB) -lpq

%.o: %.cpp
	@echo "Compiling $<"
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBPQ_FLAGS) -c $< -o $@

clean:
	@echo "Cleaning up..."
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
